// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTHitReactionAbility_ApplyRootMotion.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionDesc.h"
#include "GTHitReactionAbility_ApplyRootMotion_Airborne.generated.h"

UCLASS()
class UGTHitReactionInfo_ApplyRootMotion_Airborne : public UGTHitReactionInfo
{
	GENERATED_BODY()

public:
	UGTHitReactionInfo_ApplyRootMotion_Airborne()
	{
		ReactionTriggerTag = GTGameplayTags::Ability_HitReaction_ApplyRootMotion_Airborne;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Distance = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Height = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 1.0f;
};

/**
 * 공격자에서 피격자 방향으로 에어본 시킨다.
 */
UCLASS()
class GASTOY_API UGTHitReactionAbility_ApplyRootMotion_Airborne : public UGTHitReactionAbility_ApplyRootMotion
{
	GENERATED_BODY()

public:
	UGTHitReactionAbility_ApplyRootMotion_Airborne();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void Airborne();

	UFUNCTION()
	void OnAirborneEnded();

private:
	UPROPERTY()
	TObjectPtr<const UGTHitReactionInfo_ApplyRootMotion_Airborne> Info;
};
