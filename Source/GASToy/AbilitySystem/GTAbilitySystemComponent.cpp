// Copyright YYJ


#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"

#include "AbilitySystemGlobals.h"

UGTAbilitySystemComponent::UGTAbilitySystemComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	AbilityEndedCallbacks.AddLambda([this](UGameplayAbility* EndedAbility)
		{
			check(EndedAbility);
			OnAbilityEndedDelegate.Broadcast(EndedAbility->AbilityTags);
		});
}

UGTAbilitySystemComponent* UGTAbilitySystemComponent::GetGTAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent /*= true*/)
{
	return Cast<UGTAbilitySystemComponent>(UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor, LookForComponent));
}

void UGTAbilitySystemComponent::CancelAbilitiesByTags(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags)
{
	CancelAbilities(&WithTags, &WithoutTags);
}

void UGTAbilitySystemComponent::CancelAbilityByTag(const FGameplayTag Tag)
{
	FGameplayTagContainer Tags(Tag);
	CancelAbilities(&Tags, nullptr);
}

bool UGTAbilitySystemComponent::IsActiveAbility(UGameplayAbility* Ability) const
{
	if (!Ability)
	{
		return false;
	}
	
	const TArray<FGameplayAbilitySpec>& Specs = GetActivatableAbilities();
	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.IsActive() && Spec.Ability == Ability)
		{
			return true;
		}
	}
	
	return false;
}

bool UGTAbilitySystemComponent::IsActiveAbilityByTags(const FGameplayTagContainer& Tags) const
{
	const TArray<FGameplayAbilitySpec>& Specs = GetActivatableAbilities();
	for (const FGameplayAbilitySpec& Spec : Specs)
	{
		if (Spec.IsActive() && Spec.Ability->AbilityTags == Tags)
		{
			return true;
		}
	}

	return false;
}
