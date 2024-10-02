// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotify.h"
#include "GTAnimNotify_LookAtTargetOrInputDirection.generated.h"

/**
 * 타겟 혹은 입력 방향으로 액터를 회전시킨다.
 */
UCLASS(Meta = (DisplayName = "GTLookAtTargetOrInputDirection"))
class GASTOY_API UGTAnimNotify_LookAtTargetOrInputDirection : public UGTAnimNotify
{
	GENERATED_BODY()

public:
	UGTAnimNotify_LookAtTargetOrInputDirection();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
