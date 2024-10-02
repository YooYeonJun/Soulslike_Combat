// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotifyState.h"
#include "GTAnimNotifyState_SetOwnerTimeDilation.generated.h"

/**
 * 오너의 타임 딜레이션을 설정한다.
 */
UCLASS(Meta = (DisplayName = "GTSetOwnerTimeDilation"))
class GASTOY_API UGTAnimNotifyState_SetOwnerTimeDilation : public UGTAnimNotifyState
{
	GENERATED_BODY()

public:
	UGTAnimNotifyState_SetOwnerTimeDilation();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	float TimeDilation;
	
private:
	static void SetOwnerTimeDilationFromMesh(const USkeletalMeshComponent* MeshComponent, const float TimeDilation);
};
