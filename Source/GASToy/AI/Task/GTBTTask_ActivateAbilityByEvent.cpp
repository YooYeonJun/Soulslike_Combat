// Copyright YYJ


#include "GTBTTask_ActivateAbilityByEvent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/AbilitySystem/Ability/Dodge/GTGameplayAbility_MonsterDodge.h"

FString UGTBTTask_ActivateAbilityByEvent::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nEventTag: %s"),
		*Super::GetStaticDescription(),
		*GetEventTag().ToString());
}

bool UGTBTTask_ActivateAbilityByEvent::IsActiveAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		return OwnerASC->IsActiveAbilityByTags(FGameplayTagContainer(GetEventTag()));
	}
	
	return false;
}

bool UGTBTTask_ActivateAbilityByEvent::TryActivateAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		if (AActor* OwnerActor = OwnerASC->GetOwner())
		{
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, GetEventTag(), CreateEventData());
		}
	}
	
	return IsActiveAbility();
}

void UGTBTTask_ActivateAbilityByEvent::CancelAbility()
{
	if (UGTAbilitySystemComponent* OwnerASC = GetOwnerGTAbilitySystemComponent())
	{
		FGameplayTagContainer Tags {GetEventTag()};
		OwnerASC->CancelAbilities(&Tags);	
	}
}

bool UGTBTTask_ActivateAbilityByEvent::IsMyAbility(UGameplayAbility* Ability)
{
	return Ability && Ability->AbilityTags.HasTag(GetEventTag());
}