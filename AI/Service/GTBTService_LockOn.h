// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AI/Service/GTBTService.h"
#include "GTBTService_LockOn.generated.h"

/**
 * 범위안에 있는 플레이어 캐릭터를 락온한다.
 */
UCLASS()
class GASTOY_API UGTBTService_LockOn : public UGTBTService
{
	GENERATED_BODY()

public:
	UGTBTService_LockOn(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;
	
protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector TargetActorBlackboardKey;

	UPROPERTY(EditAnywhere)
	float RangeRadius;
	
private:
	void LockOn(UBehaviorTreeComponent& OwnerComp);

	AActor* FindTarget(AActor* OwnerActor) const;
};
