// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_Groggy.generated.h"

class UAnimMontage;
class UGameplayEffect;

/**
 * 그로기 상태가 되고 처형을 당할 수 있다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_Groggy : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_Groggy();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GroggyAndExecutedMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> IsGroggyEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float GroggyDuration;
	
private:
	void PlayMontage_GroggyLooping();
	void PlayMontage_Executed();

	UFUNCTION()
	void OnGroggyAndExecutedMontageEnded();

	void ApplyEffect_IsGroggy();
	void RemoveEffect_IsGroggy();

	void SetTimer_IsGroggy();
	void ClearTimer_IsGroggy();

	void WaitEvent_Executed();
	
	UFUNCTION()
	void OnEventReceived_Executed(FGameplayEventData EventData);

private:
	FActiveGameplayEffectHandle IsGroggyEffectHandle;
	FTimerHandle IsGroggyTimerHandle;
};


