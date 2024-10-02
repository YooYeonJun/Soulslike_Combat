// Copyright YYJ


#include "GTAreaOfEffectActor.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Components/SphereComponent.h"
#include "Kismet/KismetSystemLibrary.h"


AGTAreaOfEffectActor::AGTAreaOfEffectActor()
{
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootSceneComponent);
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
	SphereComponent->SetSphereRadius(200.0f);
	SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bCheckCollisionOnTick = false;
}

const FGameplayTag& AGTAreaOfEffectActor::GetEventTag() const
{
	return EventTag;
}

void AGTAreaOfEffectActor::SetEventTag(const FGameplayTag& InEventTag)
{
	EventTag = InEventTag;
}

void AGTAreaOfEffectActor::SetCollisionId(const FGTCollisionId& InCollisionId)
{
	CollisionId = InCollisionId;
}

void AGTAreaOfEffectActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		CheckCollisionAndSendEvent();
	}
}

void AGTAreaOfEffectActor::Init(const FGTCollisionId& InCollisionId, const FGameplayTag& InEventTag)
{
	SetCollisionId(InCollisionId);
	SetEventTag(InEventTag);
}

void AGTAreaOfEffectActor::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		CheckCollisionAndSendEvent();

		if (bCheckCollisionOnTick == false)
		{
			SetActorTickEnabled(false);
		}
	}
}

void AGTAreaOfEffectActor::CheckCollisionAndSendEvent()
{
	const FVector Start = GetActorLocation();
	const FVector End = Start + FVector::UpVector * 100.0f;
	const float Radius = SphereComponent->GetScaledSphereRadius();
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes {UCollisionProfile::Get()->ConvertToObjectType(ECC_Pawn)};
	const TArray<AActor*> ActorsToIgnore {GetOwner()};
	TArray<FHitResult> HitResults;
	
	UKismetSystemLibrary::SphereTraceMultiForObjects(this, Start, End, Radius, ObjectTypes, true, ActorsToIgnore,
		EDrawDebugTrace::None, HitResults, true);

	for (const FHitResult& HitResult : HitResults)
	{
		if (AActor* HitActor = HitResult.GetActor(); HitActors.Find(HitActor) == INDEX_NONE)
		{
			HitActors.Add(HitActor);
		
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			EventData.Instigator = GetOwner();
			EventData.Target = HitResult.GetActor();
			EventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
			
			UGTCollisionSubsystem::GetChecked(this)->SendCollisionEventToTarget(GetOwner(), EventData, CollisionId);
		}
	}
}
