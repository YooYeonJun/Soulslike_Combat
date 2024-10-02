// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotifyState.h"
#include "GTAnimNotifyState_LookAtTargetOrInputDirection.generated.h"

/**
 * 타겟 혹은 입력 방향으로 액터를 회전시킨다.
 */
UCLASS(Meta = (DisplayName = "GTLookAtTargetOrInputDirection"))
class GASTOY_API UGTAnimNotifyState_LookAtTargetOrInputDirection : public UGTAnimNotifyState
{
	GENERATED_BODY()

public:
	UGTAnimNotifyState_LookAtTargetOrInputDirection();

	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	float InterpSpeed;
	
};
