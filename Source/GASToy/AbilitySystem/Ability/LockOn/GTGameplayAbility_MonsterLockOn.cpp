// Copyright YYJ


#include "GTGameplayAbility_MonsterLockOn.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GASToy/AbilitySystem/Ability/LockOn/GTLockOnTargetComponent.h"

UGTGameplayAbility_MonsterLockOn::UGTGameplayAbility_MonsterLockOn()
{
	InitAbilityTag(GTGameplayTags::Ability_State_LockOn, true);
}

bool UGTGameplayAbility_MonsterLockOn::TryActivate(AActor* Instigator, const AActor* Target)
{
	if (Instigator && Target)
	{
		FGameplayEventData EventData;
		EventData.EventTag = GTGameplayTags::Ability_State_LockOn;
		EventData.Instigator = Instigator;
		EventData.Target = Target;

		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Instigator, EventData.EventTag, EventData);

		const UAbilitySystemComponent* InstigatorASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Instigator);
		if (InstigatorASC && InstigatorASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsLockOn))
		{
			return true;
		}
	}

	return false;
}

void UGTGameplayAbility_MonsterLockOn::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData);

	if (!TryLockOnFromActor(TriggerEventData->Target.Get()))
	{
		K2_EndAbility();
		return;
	}

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_MonsterLockOn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGTGameplayAbility_MonsterLockOn::TryLockOnFromActor(const AActor* Actor)
{
	check(Actor);

	if (UGTLockOnTargetComponent* TargetComponent = Actor->GetComponentByClass<UGTLockOnTargetComponent>())
	{
		return TryBeginLockOn(TargetComponent);
	}

	return false;
}
