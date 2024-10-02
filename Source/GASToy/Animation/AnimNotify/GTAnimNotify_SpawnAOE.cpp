// Copyright YYJ


#include "GTAnimNotify_SpawnAOE.h"

#include "GASToy/Combat/GTAreaOfEffectActor.h"

UGTAnimNotify_SpawnAOE::UGTAnimNotify_SpawnAOE()
{
	LocationOffset = FVector::ZeroVector;
	Rotation = FRotator::ZeroRotator;
	Scale = FVector::OneVector;
}

void UGTAnimNotify_SpawnAOE::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (!AOEActorClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTAnimNotify_SpawnAOE::Notify / fail, (!AOEActorClass)"));
		return;
	}

	if (!EventTag.IsValid())
	{
		UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTAnimNotify_SpawnAOE::Notify / fail, (!EventTag.IsValid())"));
		return;
	}

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		if (UWorld* World = OwnerActor->GetWorld())
		{
			FVector NewLocation = MeshComp->GetComponentLocation() + MeshComp->GetComponentRotation().RotateVector(LocationOffset);
			FRotator NewRotation = MeshComp->GetComponentRotation() + Rotation;
			FTransform Transform = FTransform::Identity;
			Transform.SetLocation(NewLocation);
			Transform.SetRotation(NewRotation.Quaternion());
			Transform.SetScale3D(Scale);
			
			AGTAreaOfEffectActor* AOEActor = World->SpawnActorDeferred<AGTAreaOfEffectActor>(AOEActorClass, Transform, OwnerActor, nullptr,
				ESpawnActorCollisionHandlingMethod::AlwaysSpawn, ESpawnActorScaleMethod::MultiplyWithRoot);

			if (!AOEActor)
			{
				UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTAnimNotify_SpawnAOE::Notify / Spawn failed"));
				return;
			}

			FGTCollisionId CollisionId;
			CollisionId.Name = CollisionIdName;
			CollisionId.InstigatorWeak = OwnerActor;

			AOEActor->Init(CollisionId, EventTag);
			AOEActor->FinishSpawning(Transform);

			// AnimMontage내에서도 파괴되게 처리
			if (World->IsEditorWorld())
			{
				AOEActor->SetLifeSpan(AOEActor->InitialLifeSpan);
			}
		}
	}
}
