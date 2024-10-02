// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTBTTask_ActivateAbility.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GTBTTask_ActivateAbilityByEvent.generated.h"

/**
 * 이벤트를 이용해 어빌리티를 실행하기 위한 베이스 클래스
 * 이벤트 데이터를 하위클래스에서 정의해야 한다.
 */
UCLASS(Abstract)
class GASTOY_API UGTBTTask_ActivateAbilityByEvent : public UGTBTTask_ActivateAbility
{
	GENERATED_BODY()

public:
	virtual FString GetStaticDescription() const override;
	
protected:
	virtual bool IsActiveAbility() override;
	virtual bool TryActivateAbility() override;
	virtual void CancelAbility() override;
	virtual bool IsMyAbility(UGameplayAbility* Ability) override;
	
	virtual FGameplayEventData CreateEventData() const PURE_VIRTUAL(
		UGTBTTask_ActivateAbilityByEvent::CreateEventData, return FGameplayEventData(); );

	virtual FGameplayTag GetEventTag() const PURE_VIRTUAL(
		UGTBTTask_ActivateAbilityByEvent::GetEventTag, return FGameplayTag(); );
};
