// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionAbility.h"
#include "GTHitReactionAbility_PlayMontage.generated.h"

class UAnimMontage;

/**
 * 
 */
UCLASS()
class GASTOY_API UGTHitReactionAbility_PlayMontage : public UGTHitReactionAbility
{
	GENERATED_BODY()

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	virtual UAnimMontage* GetMontageAndUpdateVictimRotation() PURE_VIRTUAL(UGTGameplayAbility_HitReaction_PlayMontage, return nullptr; );

private:
	void PlayMontageAndWait();
	
	UFUNCTION()
	void OnMontageEnded();
};