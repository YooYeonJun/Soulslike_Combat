// Copyright YYJ


#include "GTHitReactionAbility.h"

UGTHitReactionAbility::UGTHitReactionAbility()
{
	CancelAbilitiesWithTag.AddTag(GTGameplayTags::Ability_Action);
	BlockAbilitiesWithTag.AddTag(GTGameplayTags::Ability_Action);

	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_Immunity_HitReaction);
	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_IsGroggy);

	ActivationOwnedTags.AddTag(GTGameplayTags::Character_State_IsHitReaction);
	ActivationOwnedTags.AddTag(GTGameplayTags::Player_State_InputBlocked_Move);
}

void UGTHitReactionAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AttackerWeak = const_cast<AActor*>(TriggerEventData->Instigator.Get());
	check(AttackerWeak.IsValid());

	VictimWeak = const_cast<AActor*>(TriggerEventData->Target.Get());
	check(VictimWeak.IsValid());
}

void UGTHitReactionAbility::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}