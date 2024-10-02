// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayAbilitySpecHandle.h"
#include "GTBTTask_ActivateAbility.h"
#include "GTBTTask_ActivateAbilityByClass.generated.h"

class UGTGameplayAbility;

/**
 * 어빌리티 클래스로 어빌리티를 실행한다.
 */
UCLASS()
class GASTOY_API UGTBTTask_ActivateAbilityByClass : public UGTBTTask_ActivateAbility
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
	TSubclassOf<UGTGameplayAbility> AbilityClass;

private:
	FGameplayAbilitySpecHandle AbilitySpecHandle;
	
};
