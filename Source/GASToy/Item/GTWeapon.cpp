// Copyright YYJ

#include "GTWeapon.h"

#include "Components/CapsuleComponent.h"
#include "NiagaraComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GASToy/Component/GTCollisionCheckComponent.h"
#include "Kismet/KismetSystemLibrary.h"

AGTWeapon::AGTWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootSceneComponent;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(RootComponent);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TrailNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("TrailNiagara"));
	TrailNiagaraComponent->SetupAttachment(StaticMeshComponent);
	TrailNiagaraComponent->SetAutoActivate(false);

	CollisionCheckComponent = CreateDefaultSubobject<UGTCollisionCheckComponent>(TEXT("CollisionCheck"));
}

void AGTWeapon::BeginPlay()
{
	Super::BeginPlay();

	InitTrail();
}

void AGTWeapon::StartCollisionTest(const FGameplayTag& InEventTag)
{
	check(InEventTag.IsValid());

	EventTag = InEventTag;
	TrailNiagaraComponent->Activate();

	if (CollisionCheckComponent->CollisionCheckedDelegate.IsBoundToObject(this) == false)
	{
		CollisionCheckComponent->CollisionCheckedDelegate.BindWeakLambda(this, [this](const FGameplayTag& NewEventTag, AActor* HitActor, const FHitResult& HitResult)
		{
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			EventData.Instigator = GetOwner();
			EventData.Target = HitActor;
			EventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
					
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwner(), EventTag, EventData);
		});		
	}
	//CollisionCheckComponent->StartCollisionCheck();
}

void AGTWeapon::EndCollisionTest()
{
	TrailNiagaraComponent->Deactivate();
	//CollisionCheckComponent->EndCollisionCheck();
}

void AGTWeapon::InitTrail() const
{
	// 트레일 초기값 설정
	const FVector TopLocation = StaticMeshComponent->GetSocketLocation(TEXT("TrailTop"));
	const FVector BottomLocation = StaticMeshComponent->GetSocketLocation(TEXT("TrailBottom"));

	const FVector NewLocation = (TopLocation + BottomLocation) / 2.0f;
	const FVector Direction = TopLocation - BottomLocation;
	const FRotator NewRotation = Direction.Rotation();
	
	TrailNiagaraComponent->SetWorldLocationAndRotation(NewLocation, NewRotation);
	TrailNiagaraComponent->SetFloatParameter(TEXT("TrailWidth"), Direction.Length());
}




