// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/GTType.h"
#include "GASToy/AI/Task/GTBTTaskNode.h"
#include "GTBTTask_MoveToRandomDirection.generated.h"


/**
 * 해당 방향으로 이동 시킨다.
 */
UCLASS()
class GASTOY_API UGTBTTask_MoveToRandomDirection : public UGTBTTaskNode
{
	GENERATED_BODY()

public:
	UGTBTTask_MoveToRandomDirection(const FObjectInitializer& ObjectInitializer);

	virtual FString GetStaticDescription() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	TArray<EGTDirection> DirectionTypes;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0.001"))
	float MovementDuration;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0.0"))
	float MovementRandomDeviation;

private:
	FVector Direction;
	float RemainMovementDuration;
};
