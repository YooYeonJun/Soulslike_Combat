// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/GTType.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_Dodge.generated.h"

class UAnimMontage;
class UGameplayEffect;

/**
 * 계산된 방향으로 구르고 애니메이션을 재생한다.
 * 하위클래스에서 "CalculateMovementDirectionAndType" 를 제정의 해서 구르기에 필요한 데이터를 계산해야 한다.
 */
UCLASS(Abstract)
class GASTOY_API UGTGameplayAbility_Dodge : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_Dodge();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	virtual void CalculateMovementDirectionAndType(const FGameplayEventData* TriggerEventData,
		FVector& OutDirectionVector, EGTDirection& OutDirectionType) const PURE_VIRTUAL(
		UGTGameplayAbility_Dodge::CalculateMovementDirectionAndType, );
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EGTDirection, TObjectPtr<UAnimMontage>> DirectionTypeToMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageImmunityEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> IsDodgingEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RootMotionStrength;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float RootMotionDuration;
	
private:
	void LookAtTargetOrInputDirection();

	void PlayMontageByDirectionType(const EGTDirection DirectionType);

	void ApplyEffect_IsDodging();
	void RemoveEffect_IsDodging();
	
	void ApplyEffect_DamageImmunity();
	void RemoveEffect_DamageImmunity();

	void ApplyRootMotion(const FVector& Direction);

	UFUNCTION()
	void OnApplyRootMotionEnded();

private:
	FActiveGameplayEffectHandle IsDodgingEffectHandle;
	FActiveGameplayEffectHandle DamageImmunityEffectHandle;

};
