// Copyright YYJ


#include "GTGameplayAbility_Run.h"

#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GASToy/GTType.h"

UGTGameplayAbility_Run::UGTGameplayAbility_Run()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_Run);

	InputType = EGTAbilityInputType::Run;

	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_IsRunning);
	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_IsAttacking);
}

void UGTGameplayAbility_Run::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	WaitInputRelease();
	ApplyEffect_IsRunning();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_Run::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffect_IsRunning();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_Run::ApplyEffect_IsRunning()
{
	IsRunningEffectHandle = BP_ApplyGameplayEffectToOwner(IsRunningEffectClass);
}

void UGTGameplayAbility_Run::RemoveEffect_IsRunning()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(IsRunningEffectHandle);
}

void UGTGameplayAbility_Run::WaitInputRelease()
{
	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this);
	Task->OnRelease.AddUniqueDynamic(this, &ThisClass::OnInputReleased);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_Run::OnInputReleased(float TimeHeld)
{
	K2_EndAbility();
}
