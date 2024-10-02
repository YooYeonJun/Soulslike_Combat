// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTAnimNotify.h"
#include "GTAnimNotify_PlayCameraShake.generated.h"

class UCameraShakeBase;

/**
 * 카메라 쉐이크를 실행한다.
 */
UCLASS(Meta = (DisplayName = "GTPlayCameraShake"))
class GASTOY_API UGTAnimNotify_PlayCameraShake : public UGTAnimNotify
{
	GENERATED_BODY()

public:
	UGTAnimNotify_PlayCameraShake();
	
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> Shake;

	UPROPERTY(EditAnywhere)
	float InnerRadius;

	UPROPERTY(EditAnywhere)
	float OuterRadius;

	UPROPERTY(EditAnywhere)
	float Falloff;

	UPROPERTY(EditAnywhere)
	bool bOrientShakeTowardsEpicenter;
};
