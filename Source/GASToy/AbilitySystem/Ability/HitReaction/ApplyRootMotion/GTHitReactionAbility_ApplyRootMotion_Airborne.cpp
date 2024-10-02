// Copyright YYJ


#include "GTHitReactionAbility_ApplyRootMotion_Airborne.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionJumpForce.h"
#include "GameFramework/RootMotionSource.h"

UGTHitReactionAbility_ApplyRootMotion_Airborne::UGTHitReactionAbility_ApplyRootMotion_Airborne()
{
	InitAbilityTag(GTGameplayTags::Ability_HitReaction_ApplyRootMotion_Airborne, true);
}

void UGTHitReactionAbility_ApplyRootMotion_Airborne::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Info = GetHitReactionInfoFromEventDataChecked<UGTHitReactionInfo_ApplyRootMotion_Airborne>(TriggerEventData);

	Airborne();
}

void UGTHitReactionAbility_ApplyRootMotion_Airborne::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTHitReactionAbility_ApplyRootMotion_Airborne::Airborne()
{
	const AActor* Victim = GetVictim();
	const AActor* Attacker = GetAttacker();
	check(Victim);
	check(Attacker);
	
	const FVector AttackerToVictim = Victim->GetActorLocation() - Attacker->GetActorLocation();
	const FRotator AttackerToVictimRotation = FRotationMatrix::MakeFromX(AttackerToVictim).Rotator();
	
	UAbilityTask_ApplyRootMotionJumpForce* Task = UAbilityTask_ApplyRootMotionJumpForce::ApplyRootMotionJumpForce(
		this, TEXT("Airborne"), AttackerToVictimRotation,
		Info->Distance, Info->Height, Info->Duration,
		0.0f, true, ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector,
		0.0f, nullptr, nullptr);

	Task->OnFinish.AddUniqueDynamic(this, &ThisClass::OnAirborneEnded);
	Task->ReadyForActivation();
}

void UGTHitReactionAbility_ApplyRootMotion_Airborne::OnAirborneEnded()
{
	K2_EndAbility();
}
