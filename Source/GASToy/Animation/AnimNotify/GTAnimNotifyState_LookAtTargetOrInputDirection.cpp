// Copyright YYJ


#include "GTAnimNotifyState_LookAtTargetOrInputDirection.h"

#include "GASToy/Component/GTMovementControlComponent.h"

UGTAnimNotifyState_LookAtTargetOrInputDirection::UGTAnimNotifyState_LookAtTargetOrInputDirection()
{
	InterpSpeed = 10.0f;
}

void UGTAnimNotifyState_LookAtTargetOrInputDirection::NotifyTick(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	check(MeshComp);

	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UGTMovementControlComponent* MovementControlComponent = OwnerActor->FindComponentByClass<UGTMovementControlComponent>())
		{
			MovementControlComponent->LookAtTargetOrInputDirection(InterpSpeed);
		}
	}
}
