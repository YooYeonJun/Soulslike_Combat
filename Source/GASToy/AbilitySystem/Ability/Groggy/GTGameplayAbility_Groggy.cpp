// Copyright YYJ


#include "GTGameplayAbility_Groggy.h"

#include "Abilities/Async/AbilityAsync_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GASToy/GTType.h"

// 그로기 애니메이션을 재생하고, 처형 이벤트를 기다린다.
// 처형 이벤트 발생시 애니메이션을 재생하고 종료한다.

UGTGameplayAbility_Groggy::UGTGameplayAbility_Groggy()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_Groggy);

	CancelAbilitiesWithTag.AddTag(GTGameplayTags::Ability_Action);
	BlockAbilitiesWithTag.AddTag(GTGameplayTags::Ability_Action);

	GroggyDuration = 5.0f;
}

void UGTGameplayAbility_Groggy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	PlayMontage_GroggyLooping();
	ApplyEffect_IsGroggy();
	SetTimer_IsGroggy();
	WaitEvent_Executed();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_Groggy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffect_IsGroggy();
	ClearTimer_IsGroggy();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_Groggy::PlayMontage_GroggyLooping()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("GroggyAndExecuted"), GroggyAndExecutedMontage);

	Task->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnGroggyAndExecutedMontageEnded);
	Task->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnGroggyAndExecutedMontageEnded);
	Task->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnGroggyAndExecutedMontageEnded);
	Task->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnGroggyAndExecutedMontageEnded);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_Groggy::PlayMontage_Executed()
{
	MontageJumpToSection(TEXT("Executed"));
}

void UGTGameplayAbility_Groggy::ApplyEffect_IsGroggy()
{
	IsGroggyEffectHandle = BP_ApplyGameplayEffectToOwner(IsGroggyEffectClass);
}

void UGTGameplayAbility_Groggy::RemoveEffect_IsGroggy()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(IsGroggyEffectHandle);
}

void UGTGameplayAbility_Groggy::SetTimer_IsGroggy()
{
	GetWorld()->GetTimerManager().SetTimer(IsGroggyTimerHandle, FTimerDelegate::CreateWeakLambda(this, [this]()
	{
		K2_EndAbility();
	}), GroggyDuration, false);
}

void UGTGameplayAbility_Groggy::ClearTimer_IsGroggy()
{
	GetWorld()->GetTimerManager().ClearTimer(IsGroggyTimerHandle);
}

void UGTGameplayAbility_Groggy::OnGroggyAndExecutedMontageEnded()
{
	K2_EndAbility();
}

void UGTGameplayAbility_Groggy::WaitEvent_Executed()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
		this, GTGameplayTags::GameplayEvent_Executed);

	Task->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived_Executed);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_Groggy::OnEventReceived_Executed(FGameplayEventData EventData)
{
	PlayMontage_Executed();
	ClearTimer_IsGroggy();
}
