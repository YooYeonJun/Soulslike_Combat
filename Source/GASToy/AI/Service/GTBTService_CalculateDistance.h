// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AI/Service/GTBTService.h"
#include "GTBTService_CalculateDistance.generated.h"

/**
 * 타깃과 거리 계산을 한다.
 */
UCLASS()
class GASTOY_API UGTBTService_CalculateDistance : public UGTBTService
{
	GENERATED_BODY()

public:
	UGTBTService_CalculateDistance(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetBlackboardKey;

	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector DistanceBlackboardKey;

private:
	void CalculateDistance(UBehaviorTreeComponent& OwnerComp);
};
