// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_StandUp.generated.h"

class UAnimMontage;

/**
 * KnockDown상태일때 기상한다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_StandUp : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_StandUp();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> FrontStandUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BackStandUpMontage;

private:
	void PlayMontageAndWait();

	UFUNCTION()
	void OnMontageEnded();
};
