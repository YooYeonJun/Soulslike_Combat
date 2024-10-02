// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionAbility.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionDesc.h"
#include "GTHitReactionAbility_TimeDilation.generated.h"


UCLASS()
class UGTHitReactionInfo_TimeDilation : public UGTHitReactionInfo
{
	GENERATED_BODY()

public:
	UGTHitReactionInfo_TimeDilation()
	{
		ReactionTriggerTag = GTGameplayTags::Ability_HitReaction_TimeDilation;
	}

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSetAttackerTimeDilation = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSetVictimTimeDilation = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TimeDilation = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Duration = 0.1f;	
};

/**
 * 액터의 시간 딜레이 효과를 적용한다.
 */
UCLASS()
class GASTOY_API UGTHitReactionAbility_TimeDilation : public UGTHitReactionAbility
{
	GENERATED_BODY()

public:
	UGTHitReactionAbility_TimeDilation();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	void SetTimeDilation();
	void ResetTimeDilation();
	void SetTimeDilationAttackerAndVictim(const float TimeDilation) const;
	
private:
	FTimerHandle TimeDilationTimerHandle;

	UPROPERTY()
	TObjectPtr<const UGTHitReactionInfo_TimeDilation> Info;
};
