// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GASToy/AbilitySystem/Ability/GTOptionalObject.h"
#include "UObject/Object.h"
#include "GTHitReactionDesc.generated.h"

/**
 * 
 */

UCLASS()
class UGTHitReactionInfo : public UGTOptionalObject
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGameplayTag ReactionTriggerTag;
	
};

USTRUCT(BlueprintType)
struct FGTHitReactionDesc
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Categories = "BlockingResult"))
	FGameplayTagContainer BlockingResultRequiredTags;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced)
	TObjectPtr<UGTHitReactionInfo> Info;
};
