// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GTAnimNotify.h"
#include "GTAnimNotify_SendGameplayEventToOwner.generated.h"

/**
 * 오너에게 게임플레이 이벤트를 보낸다.
 */
UCLASS(Meta = (DisplayName = "GTSendGameplayEventToOwner"))
class GASTOY_API UGTAnimNotify_SendGameplayEventToOwner : public UGTAnimNotify
{
	GENERATED_BODY()

public:
	UGTAnimNotify_SendGameplayEventToOwner();

	virtual FString GetNotifyName_Implementation() const override;

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

	
};
