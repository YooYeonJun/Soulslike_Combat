// Copyright YYJ


#include "GTHitReactionAbility_TimeDilation.h"

UGTHitReactionAbility_TimeDilation::UGTHitReactionAbility_TimeDilation()
{
	InitAbilityTag(GTGameplayTags::Ability_HitReaction_TimeDilation, true);
}

void UGTHitReactionAbility_TimeDilation::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	Info = GetHitReactionInfoFromEventDataChecked<UGTHitReactionInfo_TimeDilation>(TriggerEventData);
	SetTimeDilation();
}

void UGTHitReactionAbility_TimeDilation::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ResetTimeDilation();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTHitReactionAbility_TimeDilation::SetTimeDilation()
{
	check(Info);

	SetTimeDilationAttackerAndVictim(Info->TimeDilation);

	const UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().SetTimer(TimeDilationTimerHandle,
		FTimerDelegate::CreateUObject(this, &ThisClass::K2_EndAbility),Info->Duration, false);
}

void UGTHitReactionAbility_TimeDilation::ResetTimeDilation()
{
	const UWorld* World = GetWorld();
	check(World);
	World->GetTimerManager().ClearTimer(TimeDilationTimerHandle);
	
	SetTimeDilationAttackerAndVictim(1.0f);
}

void UGTHitReactionAbility_TimeDilation::SetTimeDilationAttackerAndVictim(const float TimeDilation) const
{
	check(Info)

	if (Info->bSetAttackerTimeDilation)
	{
		if (AActor* Attacker = GetAttacker())
		{
			Attacker->CustomTimeDilation = TimeDilation;
		}
	}

	if (Info->bSetVictimTimeDilation)
	{
		if (AActor* Victim = GetVictim())
		{
			Victim->CustomTimeDilation = TimeDilation;
		}
	}
}
