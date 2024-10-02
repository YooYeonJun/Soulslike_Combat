// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AI/Service/GTBTService.h"
#include "GTBTService_SetMovementSpeed.generated.h"

class UGTMovementControlComponent;

/**
 * 이동속도 설정
 */
UCLASS(Abstract)
class GASTOY_API UGTBTService_SetMovementSpeed : public UGTBTService
{
	GENERATED_BODY()

public:
	UGTBTService_SetMovementSpeed(const FObjectInitializer& ObjectInitializer);

	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual float GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const PURE_VIRTUAL(
		UGTBTService_SetMovementSpeed::GetMovementSpeed, return 0.0f;);

	virtual void SetMovementSpeed(UGTMovementControlComponent& MovementControlComp, const float InSpeed) PURE_VIRTUAL(
		UGTBTService_SetMovementSpeed::SetMovementSpeed,);

protected:
	UPROPERTY(EditAnywhere)
	float Speed;

private:
	static UGTMovementControlComponent* GetMovementControlComponent(const UBehaviorTreeComponent& OwnerComp);

private:
	float PrevSpeed;
};

UCLASS()
class GASTOY_API UGTBTService_SetDefaultMovementSpeed : public UGTBTService_SetMovementSpeed
{
	GENERATED_BODY()

public:
	UGTBTService_SetDefaultMovementSpeed(const FObjectInitializer& ObjectInitializer);
	
protected:
	virtual float GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const override;
	virtual void SetMovementSpeed(UGTMovementControlComponent& MovementControlComp, const float InSpeed) override;
};

UCLASS()
class GASTOY_API UGTBTService_SetRunMovementSpeed : public UGTBTService_SetMovementSpeed
{
	GENERATED_BODY()

public:
	UGTBTService_SetRunMovementSpeed(const FObjectInitializer& ObjectInitializer);

protected:
	virtual float GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const override;
	virtual void SetMovementSpeed(UGTMovementControlComponent& MovementControlComp, const float InSpeed) override;
};

UCLASS()
class GASTOY_API UGTBTService_SetLockOnMovementSpeed : public UGTBTService_SetMovementSpeed
{
	GENERATED_BODY()

public:
	UGTBTService_SetLockOnMovementSpeed(const FObjectInitializer& ObjectInitializer);

protected:
	virtual float GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const override;
	virtual void SetMovementSpeed(UGTMovementControlComponent& MovementControlComp, const float InSpeed) override;
};
