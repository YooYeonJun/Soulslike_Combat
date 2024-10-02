// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotifyState.h"
#include "GTAnimNotifyState_SetMovementMode.generated.h"

/**
 * 캐릭터의 무브먼트 모드를 설정한다.
 */
UCLASS(Meta = (DisplayName = "GTSetMovementMode"))
class GASTOY_API UGTAnimNotifyState_SetMovementMode : public UGTAnimNotifyState
{
	GENERATED_BODY()

public:
	UGTAnimNotifyState_SetMovementMode();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementMode> MovementMode;
	
private:
	static void SetMovementModeFromMesh(const USkeletalMeshComponent* MeshComponent, const EMovementMode MovementMode);
};
