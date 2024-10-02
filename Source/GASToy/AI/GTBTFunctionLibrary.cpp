// Copyright YYJ


#include "GTBTFunctionLibrary.h"

#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"

UGTAbilitySystemComponent* UGTBTFunctionLibrary::GetGTAbilitySystemComponentFromOwnerComp(const UBehaviorTreeComponent& OwnerComp)
{
	if (const AAIController* AIOwner = OwnerComp.GetAIOwner())
	{
		if (const APawn* OwnerPawn = AIOwner->GetPawn())
		{
			return UGTAbilitySystemComponent::GetGTAbilitySystemComponentFromActor(OwnerPawn);
		}
	}

	return nullptr;
}
