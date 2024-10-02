// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTHitReactionAbility_ApplyRootMotion.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionDesc.h"
#include "GTHitReactionAbility_ApplyRootMotion_KnockBack.generated.h"

UCLASS()
class UGTHitReactionInfo_ApplyRootMotion_KnockBack : public UGTHitReactionInfo
{
	GENERATED_BODY()

public:
	UGTHitReactionInfo_ApplyRootMotion_KnockBack()
	{
		ReactionTriggerTag = GTGameplayTags::Ability_HitReaction_ApplyRootMotion_KnockBack;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Strength = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bEnableGravity = true;
};

/**

/**
 * 공격자 앞방향으로 넉백 시킨다.
 */
UCLASS()
class GASTOY_API UGTHitReactionAbility_ApplyRootMotion_KnockBack : public UGTHitReactionAbility_ApplyRootMotion
{
	GENERATED_BODY()

public:
	UGTHitReactionAbility_ApplyRootMotion_KnockBack();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void KnockBack();

	UFUNCTION()
	void OnKnockBackEnded();
	
private:
	UPROPERTY()
	TObjectPtr<const UGTHitReactionInfo_ApplyRootMotion_KnockBack> Info;
};
