// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotify.h"
#include "GTAnimNotify_CollisionScope.generated.h"

/**
 * 콜리젼 서브시스템의 콜리젼 스코프를 적용한다.
 */
UCLASS(Meta = (DisplayName = "GTCollisionScope"))
class GASTOY_API UGTAnimNotify_CollisionScope : public UGTAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FName CollisionIdName;
};
