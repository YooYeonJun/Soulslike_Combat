// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTFunctionLibrary.h"
#include "GTBTFunctionLibrary.generated.h"

class UGTAbilitySystemComponent;
class UBehaviorTreeComponent;


/**
 * 
 */
UCLASS()
class GASTOY_API UGTBTFunctionLibrary : public UBTFunctionLibrary
{
	GENERATED_BODY()

public:
	static UGTAbilitySystemComponent* GetGTAbilitySystemComponentFromOwnerComp(const UBehaviorTreeComponent& OwnerComp);
};
