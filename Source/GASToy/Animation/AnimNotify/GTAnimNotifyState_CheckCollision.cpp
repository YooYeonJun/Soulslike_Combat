// Copyright YYJ


#include "GTAnimNotifyState_CheckCollision.h"

#include "GASToy/GTType.h"
#include "GASToy/Character/GTCharacter.h"
#include "GASToy/Component/GTCollisionCheckComponent.h"
#include "GASToy/Item/GTWeapon.h"

UGTAnimNotifyState_CheckCollision::UGTAnimNotifyState_CheckCollision()
{
	EventTag = GTGameplayTags::GameplayEvent_Montage_Attack_Slash_Mid;
	Key = TEXT("Weapon");
}

void UGTAnimNotifyState_CheckCollision::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UGTCollisionCheckComponent* CollisionCheckComponent = GetOwnerCollisionCheckComponentFromMesh(MeshComp))
	{
		CollisionCheckComponent->StartCollisionCheck(Key, EventTag);
	}
}

void UGTAnimNotifyState_CheckCollision::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (UGTCollisionCheckComponent* CollisionCheckComponent = GetOwnerCollisionCheckComponentFromMesh(MeshComp))
	{
		CollisionCheckComponent->EndCollisionCheck(Key);
	}
}

UGTCollisionCheckComponent* UGTAnimNotifyState_CheckCollision::GetOwnerCollisionCheckComponentFromMesh(const USkeletalMeshComponent* MeshComponent)
{
	check(MeshComponent);
	if (const AActor* OwnerActor = MeshComponent->GetOwner())
	{
		return OwnerActor->FindComponentByClass<UGTCollisionCheckComponent>();
	}

	return nullptr;
}