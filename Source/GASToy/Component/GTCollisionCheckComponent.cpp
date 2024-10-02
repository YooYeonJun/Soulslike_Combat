// Copyright YYJ


#include "GTCollisionCheckComponent.h"

#include "Kismet/KismetSystemLibrary.h"


// Begin FGTCollisionInfo

void FGTCollisionInfo::Init(UGTCollisionCheckComponent* InOwnerComponent)
{
	check(InOwnerComponent);
	if (InOwnerComponent == OwnerComponentWeak.Get())
	{
		return;
	}
	OwnerComponentWeak = InOwnerComponent;

	CreateCollisionCheckInstances();
}

void FGTCollisionInfo::CreateCollisionCheckInstances()
{
	UGTCollisionCheckComponent* OwnerComponent = OwnerComponentWeak.Get();
	if (!OwnerComponent)
	{
		return;
	}
	
	MeshComponentWeak = OwnerComponent->GetOwner()->FindComponentByTag<UMeshComponent>(MeshComponentTag);
	const UMeshComponent* MeshComponent = MeshComponentWeak.Get();
	check(MeshComponent);
	check(MeshComponent->DoesSocketExist(StartSocketName));
	check(MeshComponent->DoesSocketExist(EndSocketName));
	check(CollisionRadius > 0.0f);

	const FVector StartLocation = MeshComponent->GetSocketLocation(StartSocketName);
	const FVector EndLocation = MeshComponent->GetSocketLocation(EndSocketName);

	const int32 InstanceCount = 1 + FVector::Distance(StartLocation, EndLocation) / CollisionRadius;	

	CollisionCheckInstances.SetNum(InstanceCount);
}

void FGTCollisionInfo::StartCollisionCheck(const FGameplayTag& InEventTag)
{
	InitCollisionCheckInstances();

	EventTag = InEventTag;
	bShouldUpdate = true;
}

void FGTCollisionInfo::InitCollisionCheckInstances()
{
	UpdateCollisionCheckInstances();
	HitActors.Empty();
}

void FGTCollisionInfo::EndCollisionCheck()
{
	bShouldUpdate = false;
}

void FGTCollisionInfo::UpdateCollisionCheckInstances()
{
	if (const UMeshComponent* MeshComponent = MeshComponentWeak.Get())
	{
		check(MeshComponent->DoesSocketExist(StartSocketName));
		check(MeshComponent->DoesSocketExist(EndSocketName));
		check(CollisionRadius > 0.0f);
	
		const FVector StartLocation = MeshComponent->GetSocketLocation(StartSocketName);
		const FVector EndLocation = MeshComponent->GetSocketLocation(EndSocketName);

		const FVector Direction = (EndLocation - StartLocation).GetSafeNormal();
	
		for (int32 Index = 0; Index < CollisionCheckInstances.Num(); Index++)
		{
			const FVector Location = StartLocation + Direction * CollisionRadius * Index;
			CollisionCheckInstances[Index].Location = Location;
		}
	}
}

void FGTCollisionInfo::CollisionCheck()
{
	if (!ShouldUpdate())
	{
		return;
	}

	const UGTCollisionCheckComponent* OwnerComponent = OwnerComponentWeak.Get();
	if (!OwnerComponent)
	{
		return;
	}
	
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes = OwnerComponent->GetObjectTypes();
	check(ObjectTypes.Num() > 0);

	TArray<FGTCollisionCheckInstance> PrevInstances = CollisionCheckInstances;
	UpdateCollisionCheckInstances();

	AActor* OwnerActor = OwnerComponent->GetOwner();

	const bool bDrawDebug = OwnerComponent->ShouldDrawDebug();
	const float DrawDebugTime = OwnerComponent->GetDrawDebugTime();
	
	for (int32 Index = 0; Index < CollisionCheckInstances.Num(); Index++)
	{
		const FVector PrevLocation = PrevInstances[Index].Location;
		const FVector CurrLocation = CollisionCheckInstances[Index].Location;

		// 이전 위치랑 동일하다면 건너뜀
		if (FVector::Distance(PrevLocation, CurrLocation) < 1.0f)
		{
			continue;
		}

		const TArray<AActor*> ActorsToIgnore = {OwnerActor, OwnerActor->GetInstigator()};
		TArray<FHitResult> HitResults;

		const EDrawDebugTrace::Type DebugTrace = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(OwnerComponent, PrevLocation,
			CurrLocation, CollisionRadius, ObjectTypes,
			false, ActorsToIgnore, DebugTrace,HitResults, true,
			FLinearColor::Red, FLinearColor::Green, DrawDebugTime);

		if (bHit)
		{
			for (const FHitResult& HitResult : HitResults)
			{
				if (AActor* HitActor = HitResult.GetActor(); HitActors.Find(HitActor) == INDEX_NONE)
				{
					HitActors.Add(HitActor);
					OwnerComponent->CollisionCheckedDelegate.ExecuteIfBound(EventTag, HitActor, HitResult);
					
					// // Debug
					// DrawDebugSphere(HitActor->GetWorld(), HitResult.ImpactPoint, 10.0f, 32, FColor::Blue, false, 1.0f);
					// DrawDebugSphere(HitActor->GetWorld(), HitResult.Location, 10.0f, 32, FColor::Red, false, 1.0f);
					// DrawDebugSphere(HitActor->GetWorld(), HitResult.TraceStart, 10.0f, 32, FColor::Green, false, 1.0f);
				}
			}
		}
	}
}

void FGTCollisionInfo::Debug_DrawCollisionInstances(UGTCollisionCheckComponent* InOwnerComp)
{
	Init(InOwnerComp);
	CreateCollisionCheckInstances();
	InitCollisionCheckInstances();
	
	if (const UGTCollisionCheckComponent* OwnerComponent = OwnerComponentWeak.Get())
	{
		for (const FGTCollisionCheckInstance& Instance : CollisionCheckInstances)
		{
			DrawDebugSphere(OwnerComponent->GetWorld(), Instance.Location, CollisionRadius, 32, FColor::Green,
				false, 1.0f);
		}
	}

	Debug_Cleanup();
}

void FGTCollisionInfo::Debug_Cleanup()
{
	OwnerComponentWeak = nullptr;
	MeshComponentWeak = nullptr;
	CollisionCheckInstances.Empty();
}

// End FGTCollisionInfo

// Begin UGTCollisionCheckComponent

UGTCollisionCheckComponent::UGTCollisionCheckComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bDrawDebug = false;
	DrawDebugTime = 0.5f;
}

void UGTCollisionCheckComponent::BeginPlay()
{
	Super::BeginPlay();
	
	for (auto& [Key, Info] : KeyToCollisionInfo)
	{
		Info.Init(this);
	}
}

void UGTCollisionCheckComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (auto& [Key, Info]: KeyToCollisionInfo)
	{
		Info.CollisionCheck();
	}
}

void UGTCollisionCheckComponent::StartCollisionCheck(const FName& Key, const FGameplayTag& EventTag)
{
	FGTCollisionInfo* Info = KeyToCollisionInfo.Find(Key);
	if (!Info)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTCollisionCheckComponent::StartCollisionCheck / fail, not found [%s]"),
			*Key.ToString());
		
		return;
	}

	Info->Init(this);
	Info->StartCollisionCheck(EventTag);
}

void UGTCollisionCheckComponent::EndCollisionCheck(const FName& Key)
{
	FGTCollisionInfo* Info = KeyToCollisionInfo.Find(Key);
	if (!Info)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTCollisionCheckComponent::StartCollisionCheck / fail, not found [%s]"),
			*Key.ToString());
		
		return;
	}

	Info->EndCollisionCheck();
}

void UGTCollisionCheckComponent::Debug_DrawCollisionInfo()
{
	FGTCollisionInfo* Info = KeyToCollisionInfo.Find(KeyForDrawCollisionInfo);
	if (!Info)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTCollisionCheckComponent::Debug_DrawCollisionInfo / fail, not found [%s]"),
			*KeyForDrawCollisionInfo.ToString());

		return;
	}

	Info->Debug_DrawCollisionInstances(this);	
}

// End UGTCollisionCheckComponent