// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_Execute.generated.h"

class UGameplayEffect;
class UAnimMontage;
class AActor;

/**
 * 이벤트데이터로 넘어온 타깃을 처형 시킨다. 
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_Execute : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_Execute();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> IsExecutingEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector TargetLocationOffset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ExecuteMontage;
	
private:
	void ApplyEffect_IsExecuting();
	void RemoveEffect_IsExecuting();

	void SetWarpTarget(const AActor* TargetActor) const;
	void SendExecutedEventToTarget(AActor* TargetActor) const;
	
	void PlayMontageAndWait();
	
	UFUNCTION()
	void OnMontageEnded();
	
private:
	FActiveGameplayEffectHandle IsExecutingEffectHandle;
};
