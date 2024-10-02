// Copyright YYJ


#include "GTBTTask_ActivateAbility.h"

#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/AI/GTBTFunctionLibrary.h"

UGTBTTask_ActivateAbility::UGTBTTask_ActivateAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_TASK_NODE_NOTIFY_FLAGS();

	bWaitForEndAbility = true;
	bSuccessIfAbilityIsActive = true;
}

EBTNodeResult::Type UGTBTTask_ActivateAbility::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	OwnerASCWeak = UGTBTFunctionLibrary::GetGTAbilitySystemComponentFromOwnerComp(OwnerComp);
	if (UGTAbilitySystemComponent* OwnerASC = OwnerASCWeak.Get())
	{
		if (IsActiveAbility())
		{
			if (bSuccessIfAbilityIsActive)
			{
				return EBTNodeResult::Succeeded;	
			}
			else
			{
				return EBTNodeResult::Failed;
			}
		}
		
		if (bWaitForEndAbility)
		{
			OwnerCompWeak = &OwnerComp;
			OwnerASC->AbilityEndedCallbacks.AddUObject(this, &ThisClass::OnAbilityEnded);			
		}
		
		if (TryActivateAbility())
		{
			if (bWaitForEndAbility)
			{
				return EBTNodeResult::InProgress;
			}
			else
			{
				return EBTNodeResult::Succeeded;
			}
		}

		if (bWaitForEndAbility)
		{
			OwnerASC->AbilityEndedCallbacks.RemoveAll(this);
		}
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UGTBTTask_ActivateAbility::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UGTAbilitySystemComponent* OwnerASC = OwnerASCWeak.Get())
	{
		OwnerASC->AbilityEndedCallbacks.RemoveAll(this);
		CancelAbility();
	}
	
	return EBTNodeResult::Aborted;
}

FString UGTBTTask_ActivateAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nbSuccessIfAbilityIsActive: %s \nWaitForEndAbility: %s"),
		*Super::GetStaticDescription(),
		bSuccessIfAbilityIsActive ? TEXT("True") : TEXT("False"),
		bWaitForEndAbility ? TEXT("True") : TEXT("False"));
}

void UGTBTTask_ActivateAbility::OnAbilityEnded(UGameplayAbility* Ability)
{
	UBehaviorTreeComponent* OwnerComp = OwnerCompWeak.Get();
	UGTAbilitySystemComponent* OwnerASC = OwnerASCWeak.Get();
	if (OwnerComp && OwnerASC)
	{
		if (IsMyAbility(Ability))
		{
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
			OwnerASC->AbilityEndedCallbacks.RemoveAll(this);
		}
	}
}
