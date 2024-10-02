// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GTAnimNotify.h"
#include "GASToy/Combat/GTCollisionSubsystem.h"
#include "GTAnimNotify_SpawnAOE.generated.h"

class AGTAreaOfEffectActor;
/**
 * AreaOfEffect 액터를 스폰한다.
 */
UCLASS(Meta = (DisplayName = "GTSpawnAOE"))
class GASTOY_API UGTAnimNotify_SpawnAOE : public UGTAnimNotify
{
	GENERATED_BODY()

public:
	UGTAnimNotify_SpawnAOE();

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AGTAreaOfEffectActor> AOEActorClass;

	UPROPERTY(EditAnywhere)
	FName CollisionIdName;
	
	UPROPERTY(EditAnywhere)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	UPROPERTY(EditAnywhere)
	FRotator Rotation;

	UPROPERTY(EditAnywhere)
	FVector Scale;
};
