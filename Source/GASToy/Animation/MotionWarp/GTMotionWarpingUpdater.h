// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GTMotionWarpingUpdater.generated.h"

class UMotionWarpingComponent;

/**
 * 워프타겟 트랜스폼을 업데이트 한다.
 */
UCLASS(Blueprintable, BlueprintType, Abstract, Const, DefaultToInstanced, EditInlineNew, CollapseCategories)
class GASTOY_API UGTMotionWarpingUpdater : public UObject
{
	GENERATED_BODY()

public:
	virtual void UpdateWarpTarget(const FName& WarpTargetName, UMotionWarpingComponent* OwnerComp, AActor* OwnerActor) {};
};

UCLASS()
class UGTMotionWarpingUpdater_LockOnTargetFront : public UGTMotionWarpingUpdater
{
	GENERATED_BODY()

public:
	UGTMotionWarpingUpdater_LockOnTargetFront();
	
	virtual void UpdateWarpTarget(const FName& WarpTargetName, UMotionWarpingComponent* OwnerComp, AActor* OwnerActor) override;

protected:
	UPROPERTY(EditAnywhere)
	float Offset;
};
