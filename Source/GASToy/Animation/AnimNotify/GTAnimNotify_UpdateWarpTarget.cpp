// Copyright YYJ


#include "GTAnimNotify_UpdateWarpTarget.h"

#include "MotionWarpingComponent.h"
#include "GASToy/Animation/MotionWarp/GTMotionWarpingUpdater.h"

UGTAnimNotify_UpdateWarpTarget::UGTAnimNotify_UpdateWarpTarget()
{
}

void UGTAnimNotify_UpdateWarpTarget::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UMotionWarpingComponent* MotionWarpingComponent = OwnerActor->FindComponentByClass<UMotionWarpingComponent>())
		{
			if (MotionWarpingUpdater)
			{
				MotionWarpingUpdater->UpdateWarpTarget(WarpTargetName, MotionWarpingComponent, OwnerActor);
			}			
		}
	}
}
