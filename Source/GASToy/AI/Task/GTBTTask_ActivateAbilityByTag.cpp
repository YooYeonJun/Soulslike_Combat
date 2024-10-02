// Copyright YYJ


#include "GTBTTask_ActivateAbilityByTag.h"

#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"

FString UGTBTTask_ActivateAbilityByTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nAbilityTags: %s"),
		*Super::GetStaticDescription(),
		*AbilityTags.ToString());
}

bool UGTBTTask_ActivateAbilityByTag::IsActiveAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		return OwnerASC->IsActiveAbilityByTags(AbilityTags);
	}

	return false;
}

bool UGTBTTask_ActivateAbilityByTag::TryActivateAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		return OwnerASC->TryActivateAbilitiesByTag(AbilityTags);
	}

	return false;
}

void UGTBTTask_ActivateAbilityByTag::CancelAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		OwnerASC->CancelAbilities(&AbilityTags);
	}
}

bool UGTBTTask_ActivateAbilityByTag::IsMyAbility(UGameplayAbility* Ability)
{
	return Ability && Ability->AbilityTags == AbilityTags;
}
