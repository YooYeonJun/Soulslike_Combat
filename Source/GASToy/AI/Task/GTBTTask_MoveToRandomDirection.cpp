// Copyright YYJ


#include "GTBTTask_MoveToRandomDirection.h"

#include "AIController.h"
#include "GASToy/GTBlueprintFunctionLibrary.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/AI/GTBTFunctionLibrary.h"
#include "GASToy/Character/GTMonsterCharacter.h"

UGTBTTask_MoveToRandomDirection::UGTBTTask_MoveToRandomDirection(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_TASK_NODE_NOTIFY_FLAGS();
	
	MovementDuration = 5.0f;
	MovementRandomDeviation = 0.0f;
	RemainMovementDuration = 0.0f;
}

FString UGTBTTask_MoveToRandomDirection::GetStaticDescription() const
{
	FString DirectionsString = TEXT("DirectionTypes: ");
	for (const EGTDirection DirectionType : DirectionTypes)
	{
		DirectionsString += FString::Printf(TEXT("%s, "), *UEnum::GetValueAsString(DirectionType));
	}
	
	return FString::Printf(TEXT("%s \n%s \nDuration: %f \nRandomDeviation: %f"), *Super::GetStaticDescription(),
		*DirectionsString, MovementDuration, MovementRandomDeviation);
}

EBTNodeResult::Type UGTBTTask_MoveToRandomDirection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (DirectionTypes.Num() > 0)
	{
		const EGTDirection DirectionType = DirectionTypes[FMath::RandRange(0, DirectionTypes.Num() - 1)];
		Direction = UGTBlueprintFunctionLibrary::CalculateDirectionVector(DirectionType);

		const float Range = FMath::RandRange(MovementDuration - MovementRandomDeviation,
			MovementDuration + MovementRandomDeviation);

		UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTBTTask_MoveToRandomDirection::ExecuteTask / Range %f"), Range);

		RemainMovementDuration = FMath::Max(0.0f, Range);

		if (UGTAbilitySystemComponent* OwnerASC = UGTBTFunctionLibrary::GetGTAbilitySystemComponentFromOwnerComp(OwnerComp))
		{
			FGameplayTagContainer Tags {GTGameplayTags::Ability_Action_Run};
			OwnerASC->CancelAbilities(&Tags);
		}

		return EBTNodeResult::InProgress;
	}

	return EBTNodeResult::Failed;
}

EBTNodeResult::Type UGTBTTask_MoveToRandomDirection::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}

void UGTBTTask_MoveToRandomDirection::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	RemainMovementDuration -= DeltaSeconds;
	if (RemainMovementDuration < 0.0f)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	if (const AAIController* AIOwner = OwnerComp.GetAIOwner())
	{
		if (AGTMonsterCharacter* OwnerMonsterCharacter = Cast<AGTMonsterCharacter>(AIOwner->GetPawn()))
		{
			const FRotator Rotation = OwnerMonsterCharacter->GetActorRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			const FVector WorldDirection = YawRotation.RotateVector(Direction);
			OwnerMonsterCharacter->Move(WorldDirection);
		}
	}
}
