// Copyright YYJ


#include "GTHitReactionAbility_PlayMontage.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

void UGTHitReactionAbility_PlayMontage::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	PlayMontageAndWait();
}

void UGTHitReactionAbility_PlayMontage::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTHitReactionAbility_PlayMontage::PlayMontageAndWait()
{
	UAnimMontage* MontageToPlay = GetMontageAndUpdateVictimRotation();
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("HitReaction Montage"), MontageToPlay);

	Task->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->ReadyForActivation();
}

void UGTHitReactionAbility_PlayMontage::OnMontageEnded()
{
	K2_EndAbility();
}
