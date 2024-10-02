// Copyright YYJ


#include "GTBTService_SetMovementSpeed.h"

#include "AIController.h"
#include "GASToy/Component/GTMovementControlComponent.h"

UGTBTService_SetMovementSpeed::UGTBTService_SetMovementSpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	
	Speed = 500.0f;
	PrevSpeed = 0.0f;
}

FString UGTBTService_SetMovementSpeed::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nSpeed: %f"), *Super::GetStaticDescription(), Speed);
}

void UGTBTService_SetMovementSpeed::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	if (UGTMovementControlComponent* MovementControlComp = GetMovementControlComponent(OwnerComp))
	{
		PrevSpeed = GetMovementSpeed(*MovementControlComp);
		SetMovementSpeed(*MovementControlComp, Speed);
	}
}

void UGTBTService_SetMovementSpeed::OnCeaseRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UGTMovementControlComponent* MovementControlComp = GetMovementControlComponent(OwnerComp))
	{
		SetMovementSpeed(*MovementControlComp, PrevSpeed);
	}
	
	Super::OnCeaseRelevant(OwnerComp, NodeMemory);
}

UGTMovementControlComponent* UGTBTService_SetMovementSpeed::GetMovementControlComponent(const UBehaviorTreeComponent& OwnerComp)
{
	if (const AAIController* AIOwner = OwnerComp.GetAIOwner())
	{
		if (const APawn* Pawn = AIOwner->GetPawn())
		{
			return Pawn->FindComponentByClass<UGTMovementControlComponent>();
		}
	}

	return nullptr;
}

UGTBTService_SetDefaultMovementSpeed::UGTBTService_SetDefaultMovementSpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Speed = 600.0f;
}

float UGTBTService_SetDefaultMovementSpeed::GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const
{
	return MovementControlComp.GetDefault_MaxMovementSpeed();
}

void UGTBTService_SetDefaultMovementSpeed::SetMovementSpeed(UGTMovementControlComponent& MovementControlComp,
	const float InSpeed)
{
	MovementControlComp.SetDefault_MaxMovementSpeed(InSpeed);
}

UGTBTService_SetRunMovementSpeed::UGTBTService_SetRunMovementSpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Speed = 500.0f;
}

float UGTBTService_SetRunMovementSpeed::GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const
{
	return MovementControlComp.GetRun_MaxMovementSpeed();
}

void UGTBTService_SetRunMovementSpeed::SetMovementSpeed(UGTMovementControlComponent& MovementControlComp,
	const float InSpeed)
{
	MovementControlComp.SetRun_MaxMovementSpeed(InSpeed);
}

UGTBTService_SetLockOnMovementSpeed::UGTBTService_SetLockOnMovementSpeed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Speed = 100.0f;
}

float UGTBTService_SetLockOnMovementSpeed::GetMovementSpeed(UGTMovementControlComponent& MovementControlComp) const
{
	return MovementControlComp.GetLockOn_MaxMovementSpeed();
}

void UGTBTService_SetLockOnMovementSpeed::SetMovementSpeed(UGTMovementControlComponent& MovementControlComp,
	const float InSpeed)
{
	MovementControlComp.SetLockOn_MaxMovementSpeed(InSpeed);
}
