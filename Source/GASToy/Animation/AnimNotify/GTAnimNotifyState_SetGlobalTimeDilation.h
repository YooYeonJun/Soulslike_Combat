// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotifyState.h"
#include "GTAnimNotifyState_SetGlobalTimeDilation.generated.h"

/**
 * 전역 타임 딜레이션을 설정한다.
 */
UCLASS(Meta = (DisplayName = "GTSetGlobalTimeDilation"))
class GASTOY_API UGTAnimNotifyState_SetGlobalTimeDilation : public UGTAnimNotifyState
{
	GENERATED_BODY()

public:
	UGTAnimNotifyState_SetGlobalTimeDilation();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	float TimeDilation;
	
	
};
