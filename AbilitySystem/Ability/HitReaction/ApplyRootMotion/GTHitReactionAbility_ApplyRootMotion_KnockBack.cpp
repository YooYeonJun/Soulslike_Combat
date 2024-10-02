// Copyright YYJ


#include "GTHitReactionAbility_ApplyRootMotion_KnockBack.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "GameFramework/RootMotionSource.h"

UGTHitReactionAbility_ApplyRootMotion_KnockBack::UGTHitReactionAbility_ApplyRootMotion_KnockBack()
{
	InitAbilityTag(GTGameplayTags::Ability_HitReaction_ApplyRootMotion_KnockBack, true);
}

void UGTHitReactionAbility_ApplyRootMotion_KnockBack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Info = GetHitReactionInfoFromEventDataChecked<UGTHitReactionInfo_ApplyRootMotion_KnockBack>(TriggerEventData);

	KnockBack();
}

void UGTHitReactionAbility_ApplyRootMotion_KnockBack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTHitReactionAbility_ApplyRootMotion_KnockBack::KnockBack()
{
	const AActor* Victim = GetVictim();
	const AActor* Attacker = GetAttacker();
	check(Victim);
	check(Attacker);

	const FVector AttackerToVictim = Victim->GetActorLocation() - Attacker->GetActorLocation();
	const FVector WorldDirection = AttackerToVictim.GetSafeNormal();
	
	UAbilityTask_ApplyRootMotionConstantForce* Task = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
			this, TEXT("KnockBack"), WorldDirection, Info->Strength, Info->Duration,
			false, nullptr, ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector,
			0.0f, Info->bEnableGravity);

	Task->OnFinish.AddUniqueDynamic(this, &ThisClass::OnKnockBackEnded);
	Task->ReadyForActivation();
}

void UGTHitReactionAbility_ApplyRootMotion_KnockBack::OnKnockBackEnded()
{
	K2_EndAbility();
}
