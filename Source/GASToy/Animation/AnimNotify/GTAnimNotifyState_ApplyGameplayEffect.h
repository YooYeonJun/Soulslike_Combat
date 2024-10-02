// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "ActiveGameplayEffectHandle.h"
#include "GTAnimNotifyState.h"
#include "GTAnimNotifyState_ApplyGameplayEffect.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
/**
 * 게임플레이 이펙트를 적용한다.
 */
UCLASS(Meta = (DisplayName = "GTApplyGameplayEffect"))
class GASTOY_API UGTAnimNotifyState_ApplyGameplayEffect : public UGTAnimNotifyState
{
	GENERATED_BODY()

public:
	UGTAnimNotifyState_ApplyGameplayEffect();

	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UGameplayEffect> EffectClass;
	
private:
	static UAbilitySystemComponent* GetAbilitySystemComponentFromMeshOwner(const USkeletalMeshComponent* MeshComponent);

private:
	FActiveGameplayEffectHandle EffectHandle;
};
