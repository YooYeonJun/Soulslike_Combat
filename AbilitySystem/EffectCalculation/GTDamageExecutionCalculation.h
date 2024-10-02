// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "GTDamageExecutionCalculation.generated.h"

/**
 * 데미지 계산을 한다. 막기 결과에 따라 계산한다.
 */
UCLASS()
class GASTOY_API UGTDamageExecutionCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UGTDamageExecutionCalculation();

	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
