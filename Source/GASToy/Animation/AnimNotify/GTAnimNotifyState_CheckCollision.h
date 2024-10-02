// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GTAnimNotifyState.h"
#include "GTAnimNotifyState_CheckCollision.generated.h"

class UGTCollisionCheckComponent;
class AGTWeapon;

/**
 * 충돌 검사를 한다.
 */
UCLASS(Meta = (DisplayName = "GTCheckCollision"))
class GASTOY_API UGTAnimNotifyState_CheckCollision : public UGTAnimNotifyState
{
	GENERATED_BODY()

public:
	UGTAnimNotifyState_CheckCollision();
	
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere)
	FName Key;
	
private:
	static UGTCollisionCheckComponent* GetOwnerCollisionCheckComponentFromMesh(const USkeletalMeshComponent* MeshComponent);
};