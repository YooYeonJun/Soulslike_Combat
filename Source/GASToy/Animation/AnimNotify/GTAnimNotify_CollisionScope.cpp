// Copyright YYJ


#include "GTAnimNotify_CollisionScope.h"

#include "GASToy/Combat/GTCollisionSubsystem.h"

void UGTAnimNotify_CollisionScope::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (const AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UGTCollisionSubsystem* CollisionSubsystem = UGTCollisionSubsystem::Get(OwnerActor))
		{
			FGTCollisionId Id;
			Id.Name = CollisionIdName;
			Id.InstigatorWeak = OwnerActor;

			// 이전 스코프를 끝내고 새로 시작함.
			// 이후에 동일한 아이디로 충돌 이벤트가 중복될 경우 처음 한 번만 처리가 된다.
			CollisionSubsystem->EndScope(Id);
			CollisionSubsystem->BeginScope(Id);
		}
	}
}
