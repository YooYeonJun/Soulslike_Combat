// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GTBTService.generated.h"

class UAbilitySystemComponent;

/**
 * 서비스 베이스 클래스 
 */
UCLASS(Abstract)
class GASTOY_API UGTBTService : public UBTService
{
	GENERATED_BODY()

public:
	UGTBTService(const FObjectInitializer& ObjectInitializer);
	
};
