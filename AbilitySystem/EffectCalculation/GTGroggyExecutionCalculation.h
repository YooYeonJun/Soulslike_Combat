// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GTGroggyExecutionCalculation.generated.h"

/**
 * 그로기를 계산한다. 막기 결과에 따라 계산한다.
 */
UCLASS()
class GASTOY_API UGTGroggyExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGTGroggyExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
