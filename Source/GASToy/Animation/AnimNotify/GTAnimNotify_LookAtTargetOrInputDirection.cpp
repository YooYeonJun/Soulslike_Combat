// Copyright YYJ


#include "GTAnimNotify_LookAtTargetOrInputDirection.h"

#include "GASToy/Component/GTMovementControlComponent.h"

UGTAnimNotify_LookAtTargetOrInputDirection::UGTAnimNotify_LookAtTargetOrInputDirection()
{
}

void UGTAnimNotify_LookAtTargetOrInputDirection::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	check(MeshComp);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UGTMovementControlComponent* MovementControlComponent = OwnerActor->FindComponentByClass<UGTMovementControlComponent>())
		{
			MovementControlComponent->LookAtTargetOrInputDirection(0.0f);
		}
	}
}
