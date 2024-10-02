// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTGameplayAbility_ScanGroggy.generated.h"

class UGameplayEffect;
class AActor;

/**
 * 그로기 대상을 찾아 처형어빌리티에 처형을 위임한다. 
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_ScanGroggy : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_ScanGroggy();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> CanExecuteEffectClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScanRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScanRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float ScanAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ScanObjectTypes;
	
private:
	void ScanGroggyActor();
	void SetTimer_ScanGroggyActor();
	void ClearTimer_ScanGroggyActor();

	void ApplyEffect_CanExecute();
	void RemoveEffect_CanExecute();

	void WaitInputPress();

	UFUNCTION()
	void OnInputPressed(float TimeWaited);

	static bool IsGroggyActor(const AActor* Actor);
	
private:
	FTimerHandle ScanGroggyActorTimerHandle;
	
	TWeakObjectPtr<AActor> GroggyActorWeak;

	FActiveGameplayEffectHandle CanExecuteEffectHandle;
};
