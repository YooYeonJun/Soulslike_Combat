// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTBTTask_ActivateAbility.h"
#include "GTBTTask_ActivateAbilityByTag.generated.h"

class UGameplayAbility;
/**
 * 어빌리티 태그로 어빌리티를 실행한다.
 */
UCLASS()
class GASTOY_API UGTBTTask_ActivateAbilityByTag : public UGTBTTask_ActivateAbility
{
	GENERATED_BODY()

public:
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool IsActiveAbility() override;
	virtual bool TryActivateAbility() override;
	virtual void CancelAbility() override;
	virtual bool IsMyAbility(UGameplayAbility* Ability) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FGameplayTagContainer AbilityTags;
};
