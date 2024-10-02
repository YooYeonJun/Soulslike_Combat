// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_LockOn.generated.h"

class UGTMovementControlComponent;
class UGTLockOnTargetComponent;

/**
 * 락온 베이스 클래스, 락온 기능 구현
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_LockOn : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_LockOn();

protected:
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	UFUNCTION(BlueprintCallable)
	bool TryBeginLockOn(UGTLockOnTargetComponent* InTarget);

	UFUNCTION(BlueprintCallable)
	bool TryEndLockOn();

	UFUNCTION(BlueprintPure)
	UGTLockOnTargetComponent* GetTarget() const { return TargetWeak.Get(); }

	UFUNCTION(BlueprintPure)
	UGTMovementControlComponent* GetOwnerMovementControlComponent() const;

private:
	void OnLockOnEnded();

	void ApplyEffect_IsLockOn();
	void RemoveEffect_IsLockOn();

private:
	FActiveGameplayEffectHandle IsLockOnEffectHandle;
	
	TWeakObjectPtr<UGTLockOnTargetComponent> TargetWeak;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayEffect> IsLockOnEffectClass;
};
