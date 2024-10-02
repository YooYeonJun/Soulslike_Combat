// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTHitReactionAbility_PlayMontage.h"
#include "GASToy/AbilitySystem/Ability/HitReaction/GTHitReactionDesc.h"
#include "GTHitReactionAbility_PlayMontage_Airborne.generated.h"

UCLASS()
class UGTHitReactionInfo_PlayMontage_Airborne : public UGTHitReactionInfo
{
	GENERATED_BODY()

public:
	UGTHitReactionInfo_PlayMontage_Airborne()
	{
		ReactionTriggerTag = GTGameplayTags::Ability_HitReaction_PlayMontage_Airborne;
	}
};


/**
 * 
 */
UCLASS()
class GASTOY_API UGTHitReactionAbility_PlayMontage_Airborne : public UGTHitReactionAbility_PlayMontage
{
	GENERATED_BODY()

public:
	UGTHitReactionAbility_PlayMontage_Airborne();
	
protected:
	virtual UAnimMontage* GetMontageAndUpdateVictimRotation() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> BackwardAirborneMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ForwardAirborneMontage;

};
