// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_MontageAttack.generated.h"

class UGameplayEffect;
class UAnimMontage;

/**
 * 몽타주를 재생하고, 이벤트를 받아 데미지를 적용한다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_MontageAttack : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_MontageAttack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> IsAttackingEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> MontageToPlay;

	UFUNCTION(BlueprintNativeEvent)
	UAnimMontage* GetMontageToPlay();
	
private:
	void WaitEvent_AttackEnd();

	UFUNCTION()
	void OnEventReceived_AttackEnd(FGameplayEventData EventData);

	void ApplyEffect_IsAttacking();
	void RemoveEffect_IsAttacking();

	void PlayMontageAndWait();

	UFUNCTION()
	void OnMontageEnded();

private:
	FActiveGameplayEffectHandle IsAttackingEffectHandle;
};


