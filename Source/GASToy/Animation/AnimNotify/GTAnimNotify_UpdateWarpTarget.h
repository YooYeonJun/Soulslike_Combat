// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotify.h"
#include "GTAnimNotify_UpdateWarpTarget.generated.h"

class UGTMotionWarpingUpdater;
/**
 * UGTMotionWarpingUpdater를 이용해 워프타겟 트랜스폼을 업데이트 한다.
 */
UCLASS(Meta = (DisplayName = "GTUpdateWarpTarget"))
class GASTOY_API UGTAnimNotify_UpdateWarpTarget : public UGTAnimNotify
{
	GENERATED_BODY()

public:
	UGTAnimNotify_UpdateWarpTarget();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UGTMotionWarpingUpdater> MotionWarpingUpdater;

	UPROPERTY(EditAnywhere)
	FName WarpTargetName;
};
