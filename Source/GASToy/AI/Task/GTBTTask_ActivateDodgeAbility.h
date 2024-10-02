// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AI/Task/GTBTTask_ActivateAbilityByEvent.h"
#include "GTBTTask_ActivateDodgeAbility.generated.h"

class UGameplayAbility;
class UGTOptionalObject_MonsterDodge;
class UAbilitySystemComponent;

/**
 * 닷지 데이터를 이용해 어빌리티를 실행한다. 
 */
UCLASS()
class GASTOY_API UGTBTTask_ActivateDodgeAbility : public UGTBTTask_ActivateAbilityByEvent
{
	GENERATED_BODY()

public:
	UGTBTTask_ActivateDodgeAbility(const FObjectInitializer& ObjectInitializer);
	virtual FString GetStaticDescription() const override;

protected:
	virtual FGameplayEventData CreateEventData() const override;
	virtual FGameplayTag GetEventTag() const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UGTOptionalObject_MonsterDodge> MonsterDodgeData;

};
