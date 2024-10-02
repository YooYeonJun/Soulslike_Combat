// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_Run.generated.h"

class UGameplayEffect;

/**
 * 달리기 상태를 부여하고, 입력이 해제될때 상태 제거.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_Run : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_Run();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> IsRunningEffectClass;
	
private:
	void ApplyEffect_IsRunning();
	void RemoveEffect_IsRunning();

	void WaitInputRelease();

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

private:
	FActiveGameplayEffectHandle IsRunningEffectHandle;
};