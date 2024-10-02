// Copyright YYJ


#include "GTGameplayAbility_MontageAttack.h"

#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GASToy/GTType.h"

UGTGameplayAbility_MontageAttack::UGTGameplayAbility_MontageAttack()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_Attack);
	InputType = EGTAbilityInputType::Attack;
}

void UGTGameplayAbility_MontageAttack::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	WaitEvent_ApplyDamage(GTGameplayTags::GameplayEvent_Montage, nullptr, false, false);
	WaitEvent_AttackEnd();

	PlayMontageAndWait();
	ApplyEffect_IsAttacking();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_MontageAttack::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffect_IsAttacking();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

UAnimMontage* UGTGameplayAbility_MontageAttack::GetMontageToPlay_Implementation()
{
	return MontageToPlay;
}

void UGTGameplayAbility_MontageAttack::WaitEvent_AttackEnd()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
this, GTGameplayTags::GameplayEvent_AttackEnd, nullptr, true, true);

	Task->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived_AttackEnd);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_MontageAttack::OnEventReceived_AttackEnd(FGameplayEventData EventData)
{
	RemoveEffect_IsAttacking();
}

void UGTGameplayAbility_MontageAttack::ApplyEffect_IsAttacking()
{
	IsAttackingEffectHandle = BP_ApplyGameplayEffectToOwner(IsAttackingEffectClass);
}

void UGTGameplayAbility_MontageAttack::RemoveEffect_IsAttacking()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(IsAttackingEffectHandle);
}

void UGTGameplayAbility_MontageAttack::PlayMontageAndWait()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("Attack"), GetMontageToPlay());

	Task->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_MontageAttack::OnMontageEnded()
{
	K2_EndAbility();
}
