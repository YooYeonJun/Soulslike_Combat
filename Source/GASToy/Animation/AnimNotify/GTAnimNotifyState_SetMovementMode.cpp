// Copyright YYJ


#include "GTAnimNotifyState_SetMovementMode.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UGTAnimNotifyState_SetMovementMode::UGTAnimNotifyState_SetMovementMode()
{
	MovementMode = MOVE_Flying;
}

void UGTAnimNotifyState_SetMovementMode::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	SetMovementModeFromMesh(MeshComp, MovementMode);
}

void UGTAnimNotifyState_SetMovementMode::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	SetMovementModeFromMesh(MeshComp, EMovementMode::MOVE_Falling);
}

void UGTAnimNotifyState_SetMovementMode::SetMovementModeFromMesh(const USkeletalMeshComponent* MeshComponent, const EMovementMode MovementMode)
{
	check(MeshComponent);
	if (const ACharacter* OwnerCharacter = Cast<ACharacter>(MeshComponent->GetOwner()))
	{
		if (UCharacterMovementComponent* OwnerCMC = OwnerCharacter->GetCharacterMovement())
		{
			OwnerCMC->SetMovementMode(MovementMode);
		}
	}
}
