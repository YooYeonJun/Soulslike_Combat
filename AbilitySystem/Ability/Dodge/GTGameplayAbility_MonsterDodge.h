// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/Dodge/GTGameplayAbility_Dodge.h"
#include "GASToy/AbilitySystem/Ability/GTOptionalObject.h"
#include "GTGameplayAbility_MonsterDodge.generated.h"

UCLASS()
class UGTOptionalObject_MonsterDodge : public UGTOptionalObject
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGTDirection DodgeDirectionType;
};

/**
 * 게임플레이 이벤트 데이터를 이용하여 구른다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_MonsterDodge : public UGTGameplayAbility_Dodge
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_MonsterDodge();
	
protected:
	virtual void CalculateMovementDirectionAndType(const FGameplayEventData* TriggerEventData, FVector& OutDirectionVector,
		EGTDirection& OutDirectionType) const override;
};
