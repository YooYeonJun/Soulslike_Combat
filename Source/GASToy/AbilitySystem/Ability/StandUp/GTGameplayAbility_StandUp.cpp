// Copyright YYJ


#include "GTGameplayAbility_StandUp.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GASToy/GTType.h"

UGTGameplayAbility_StandUp::UGTGameplayAbility_StandUp()
{
	InputType = EGTAbilityInputType::Dodge;

	InitAbilityTag(GTGameplayTags::Ability_Action_StandUp);

	ActivationRequiredTags.AddTag(GTGameplayTags::Character_State_IsKnockDown);
}

void UGTGameplayAbility_StandUp::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	PlayMontageAndWait();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_StandUp::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_StandUp::PlayMontageAndWait()
{
	const bool bFront = GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(
		GTGameplayTags::Character_State_IsKnockDown_Front);
	UAnimMontage* MontageToPlay = bFront ? FrontStandUpMontage : BackStandUpMontage;
	
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("StandUp"), MontageToPlay);
	
	Task->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	
	Task->ReadyForActivation();
}

void UGTGameplayAbility_StandUp::OnMontageEnded()
{
	K2_EndAbility();
}
