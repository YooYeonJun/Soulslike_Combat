// Copyright YYJ


#include "GTAnimNotifyState_SetOwnerTimeDilation.h"

UGTAnimNotifyState_SetOwnerTimeDilation::UGTAnimNotifyState_SetOwnerTimeDilation()
{
	TimeDilation = 0.4f;
}

void UGTAnimNotifyState_SetOwnerTimeDilation::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	SetOwnerTimeDilationFromMesh(MeshComp, TimeDilation);
}

void UGTAnimNotifyState_SetOwnerTimeDilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	SetOwnerTimeDilationFromMesh(MeshComp, 1.0f);
}

void UGTAnimNotifyState_SetOwnerTimeDilation::SetOwnerTimeDilationFromMesh(const USkeletalMeshComponent* MeshComponent, const float TimeDilation)
{
	check(MeshComponent);

	if (AActor* OwnerActor = MeshComponent->GetOwner())
	{
		OwnerActor->CustomTimeDilation = TimeDilation;
	}
}
