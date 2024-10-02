// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/Dodge/GTGameplayAbility_Dodge.h"
#include "GTGameplayAbility_PlayerDodge.generated.h"

/**
 * 플레이어 입력방향으로 구른다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_PlayerDodge : public UGTGameplayAbility_Dodge
{
	GENERATED_BODY()

protected:
	virtual void CalculateMovementDirectionAndType(const FGameplayEventData* TriggerEventData, FVector& OutDirectionVector,
		EGTDirection& OutDirectionType) const override;
};