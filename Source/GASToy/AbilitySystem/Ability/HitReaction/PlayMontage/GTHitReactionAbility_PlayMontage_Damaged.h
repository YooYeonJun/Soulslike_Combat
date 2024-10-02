// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTHitReactionAbility_PlayMontage.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionDesc.h"
#include "GTHitReactionAbility_PlayMontage_Damaged.generated.h"


UCLASS()
class UGTHitReactionInfo_PlayMontage_Damaged : public UGTHitReactionInfo
{
	GENERATED_BODY()

public:
	UGTHitReactionInfo_PlayMontage_Damaged()
	{
		ReactionTriggerTag = GTGameplayTags::Ability_HitReaction_PlayMontage_Damaged;
	}
};


/**
 * 
 */
UCLASS()
class GASTOY_API UGTHitReactionAbility_PlayMontage_Damaged : public UGTHitReactionAbility_PlayMontage
{
	GENERATED_BODY()

public:
	UGTHitReactionAbility_PlayMontage_Damaged();

protected:
	virtual UAnimMontage* GetMontageAndUpdateVictimRotation() override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TObjectPtr<UAnimMontage>> MontagesToPlay;
};
