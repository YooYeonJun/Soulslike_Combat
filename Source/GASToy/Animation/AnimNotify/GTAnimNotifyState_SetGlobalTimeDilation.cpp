// Copyright YYJ


#include "GTAnimNotifyState_SetGlobalTimeDilation.h"

#include "Kismet/GameplayStatics.h"

UGTAnimNotifyState_SetGlobalTimeDilation::UGTAnimNotifyState_SetGlobalTimeDilation()
{
	TimeDilation = 0.4f;
}

void UGTAnimNotifyState_SetGlobalTimeDilation::NotifyBegin(USkeletalMeshComponent* MeshComp,
	UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	UGameplayStatics::SetGlobalTimeDilation(MeshComp, TimeDilation);
}

void UGTAnimNotifyState_SetGlobalTimeDilation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	UGameplayStatics::SetGlobalTimeDilation(MeshComp, 1.0f);
}
