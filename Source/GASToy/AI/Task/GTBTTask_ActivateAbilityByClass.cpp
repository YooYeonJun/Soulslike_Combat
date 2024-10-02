// Copyright YYJ


#include "GTBTTask_ActivateAbilityByClass.h"

#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"

FString UGTBTTask_ActivateAbilityByClass::GetStaticDescription() const
{
	FString Description = TEXT("None"); 
	if (UGTGameplayAbility* GTGameplayAbility = AbilityClass.GetDefaultObject())
	{
		Description = GTGameplayAbility->GetDescription();
	}
	
	return FString::Printf(TEXT("%s \nAbilityClass: %s \nDescription: %s"),
		*Super::GetStaticDescription(),
		*GetNameSafe(AbilityClass.GetDefaultObject()),
		*Description);
}

bool UGTBTTask_ActivateAbilityByClass::IsActiveAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		return OwnerASC->IsActiveAbility(AbilityClass.GetDefaultObject());
	}

	return false;
}

bool UGTBTTask_ActivateAbilityByClass::TryActivateAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		FGameplayAbilitySpec Spec(AbilityClass);
		AbilitySpecHandle = OwnerASC->GiveAbilityAndActivateOnce(Spec);
		return AbilitySpecHandle.IsValid();
	}

	return false;
}

void UGTBTTask_ActivateAbilityByClass::CancelAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		OwnerASC->CancelAbilityHandle(AbilitySpecHandle);
	}
}

bool UGTBTTask_ActivateAbilityByClass::IsMyAbility(UGameplayAbility* Ability)
{
	return Ability && Ability->GetCurrentAbilitySpecHandle() == AbilitySpecHandle;
}
