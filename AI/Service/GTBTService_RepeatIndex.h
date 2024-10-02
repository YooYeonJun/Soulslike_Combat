// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AI/Service/GTBTService.h"
#include "GTBTService_RepeatIndex.generated.h"

/**
 * 인덱스를 1씩 증가시키고 Max를 초과하면 0으로 설정한다.
 */
UCLASS()
class GASTOY_API UGTBTService_RepeatIndex : public UGTBTService
{
	GENERATED_BODY()

public:
	UGTBTService_RepeatIndex(const FObjectInitializer& ObjectInitializer);

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector IndexBlackboardKey;
	
	UPROPERTY(EditAnywhere)
	int32 Max;

private:
	void RepeatIndex(UBehaviorTreeComponent& OwnerComp) const;
};
