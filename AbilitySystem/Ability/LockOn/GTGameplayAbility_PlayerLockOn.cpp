// Copyright YYJ


#include "GTGameplayAbility_PlayerLockOn.h"

#include "GTLockOnTargetComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Kismet/KismetSystemLibrary.h"

UGTGameplayAbility_PlayerLockOn::UGTGameplayAbility_PlayerLockOn()
{
	TraceRadius = 600.0f;
	TraceDistance = 3000.0f;

	bDrawDebug = false;
	DrawDebugTime = 1.0f;
}

void UGTGameplayAbility_PlayerLockOn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	WaitInputPress();
	FindTarget();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_PlayerLockOn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearTimer_CheckDistance();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_PlayerLockOn::WaitInputPress()
{
	UAbilityTask_WaitInputPress* Task = UAbilityTask_WaitInputPress::WaitInputPress(
		this, false);

	Task->OnPress.AddDynamic(this, &ThisClass::OnInputPressed);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_PlayerLockOn::FindTarget()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	check(Avatar);

	const APlayerController* PlayerController = GetPlayerControllerFromActorInfo();
	check(PlayerController);

	const FVector Start = Avatar->GetActorLocation();
	const FVector End = Start + PlayerController->GetControlRotation().Vector() * TraceDistance;

	const TArray<AActor*> ActorsToIgnore = {Avatar};
	const EDrawDebugTrace::Type DrawDebugType = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UCollisionProfile::Get()->ConvertToObjectType(ECC_Pawn)};

	TArray<FHitResult> OutHits;

	const bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(this, Start, End, TraceRadius, ObjectTypes,
		false, ActorsToIgnore, DrawDebugType, OutHits, true,
		FLinearColor::Red, FLinearColor::Green, DrawDebugTime);

	for (const FHitResult& HitResult : OutHits)
	{
		if (const AActor* HitActor = HitResult.GetActor())
		{
			if (UGTLockOnTargetComponent* LockOnTargetComponent = HitActor->FindComponentByClass<UGTLockOnTargetComponent>())
			{
				if (TryBeginLockOn(LockOnTargetComponent))
				{
					SetTimer_CheckDistance();
					return;
				}
			}
		}
	}

	K2_EndAbility();
}

void UGTGameplayAbility_PlayerLockOn::SetTimer_CheckDistance()
{
	const UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().SetTimer(CheckDistanceTimerHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::CheckDistance),
		1.0f, true);
}

void UGTGameplayAbility_PlayerLockOn::ClearTimer_CheckDistance()
{
	const UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(CheckDistanceTimerHandle);
}

void UGTGameplayAbility_PlayerLockOn::CheckDistance()
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
	const UGTLockOnTargetComponent* TargetComponent = GetTarget();
	if (!Avatar || !TargetComponent)
	{
		K2_EndAbility();
		return;
	}

	const float Dist = FVector::Dist2D(Avatar->GetActorLocation(), TargetComponent->GetComponentLocation());
	if (Dist > TraceDistance)
	{
		K2_EndAbility();
	}
}

void UGTGameplayAbility_PlayerLockOn::OnInputPressed(float TimeWaited)
{
	K2_EndAbility();
}
