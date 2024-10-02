// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_GuardAndParry.generated.h"

class UAnimMontage;
class UGameplayEffect;
class UAbilityTask_WaitGameplayEvent;
class UAbilityTask_WaitInputPress;
class UAbilityTask_WaitInputRelease;
class UAbilityTask_PlayMontageAndWait;
class UAbilityTask_WaitGameplayEffectRemoved;

/**
 * 가드와 패링을 한다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_GuardAndParry : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_GuardAndParry();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GuardStartAndLoopLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GuardStartAndLoopRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GuardSuccessLeftMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> GuardSuccessRightMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ParrySuccessLeftDownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ParrySuccessRightDownMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ParrySuccessLeftUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> ParrySuccessRightUpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> CanGuardEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> CanParryEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GroggyToAttackerEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float OffsetForCheckUpImpact;
	
private:
	void Cleanup();
	
	void PlayMontage_GuardStart();
	void PlayMontage_GuardLoop();
	void PlayMontage_GuardSuccess();
	void PlayMontage_ParrySuccess();

	void ApplyEffectAndWaitRemove_CanParry();
	void RemoveEffect_CanParry();

	UFUNCTION()
    void OnCanParryEffectRemoved(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo);

	void ApplyEffect_CanGuard();
	void RemoveEffect_CanGuard();

	bool IsRightImpactPoint(const FVector& ImpactPoint) const;
	bool IsUpImpactPoint(const FVector& ImpactPoint) const;
	
	void ApplyGroggyEffectToAttacker(const FGameplayEventData& EventData) const;

	void WaitEvent_BlockingResult();

	UFUNCTION()
	void OnEventReceived_BlockingResult(FGameplayEventData EventData);

	void WaitInputPress();

	UFUNCTION()
	void OnInputPressed(float TimeWaited);

	void WaitInputRelease();

	UFUNCTION()
	void OnInputReleased(float TimeHeld);

	UFUNCTION()
	void OnSuccessMontageEnded();

private:
	bool bIsRightImpactOfLastHit;
	bool bIsUpImpactOfLastHit;
	bool bIsBlockingSuccess;
	bool bIsWaitToEndAbilty;

	FActiveGameplayEffectHandle CanParryEffectHandle;
	FActiveGameplayEffectHandle CanGuardEffectHandle;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask_GuardStartAndLoop;
	
	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask_GuardSuccess;

	UPROPERTY()
	TObjectPtr<UAbilityTask_PlayMontageAndWait> MontageTask_ParrySuccess;
};
