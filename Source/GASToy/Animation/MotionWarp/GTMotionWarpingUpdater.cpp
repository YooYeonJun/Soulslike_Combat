// Copyright YYJ


#include "GTMotionWarpingUpdater.h"

#include "MotionWarpingComponent.h"
#include "GASToy/Component/GTMovementControlComponent.h"

UGTMotionWarpingUpdater_LockOnTargetFront::UGTMotionWarpingUpdater_LockOnTargetFront()
{
	Offset = 100.0f;
}

void UGTMotionWarpingUpdater_LockOnTargetFront::UpdateWarpTarget(const FName& WarpTargetName,
	UMotionWarpingComponent* OwnerComp, AActor* OwnerActor)
{
	if (!OwnerComp)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTMotionWarpingUpdater_LockOnTargetFront::UpdateWarpTarget / fail, (!OwnerComp)"));
		return;
	}

	if (!OwnerActor)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTMotionWarpingUpdater_LockOnTargetFront::UpdateWarpTarget / fail, (!OwnerActor)"));
		return;
	}
	
	UGTMovementControlComponent* MovementControlComponent = OwnerActor->FindComponentByClass<UGTMovementControlComponent>();
	if (!MovementControlComponent)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTMotionWarpingUpdater_LockOnTargetFront::UpdateWarpTarget / fail, (!MovementControlComponent)"));
		return;
	}

	AActor* LockOnTarget = MovementControlComponent->GetLockOnTarget();
	if (!LockOnTarget)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTMotionWarpingUpdater_LockOnTargetFront::UpdateWarpTarget / fail, (!LockOnTarget)"));
		return;
	}
	
	FVector LockOnTargetToOwner = OwnerActor->GetActorLocation() - LockOnTarget->GetActorLocation();
	LockOnTargetToOwner.Z = 0;
	LockOnTargetToOwner.Normalize();
			
	const FVector WarpLocation = LockOnTarget->GetActorLocation() + LockOnTargetToOwner * Offset - FVector::UpVector * 90.0f;
	OwnerComp->AddOrUpdateWarpTargetFromLocation(WarpTargetName, WarpLocation);

	// // Debug
	// DrawDebugSphere(OwnerActor->GetWorld(), WarpLocation, 50, 32, FColor::Red, false, 1.0f);
}
