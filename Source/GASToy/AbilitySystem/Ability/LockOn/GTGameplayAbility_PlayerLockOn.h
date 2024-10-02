// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/LockOn/GTGameplayAbility_LockOn.h"
#include "GTGameplayAbility_PlayerLockOn.generated.h"

/**
 * 플레이어 카메라 정면 방향으로 충돌검사를 통해 락온 대상을 찾고 락온 한다.
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_PlayerLockOn : public UGTGameplayAbility_LockOn
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_PlayerLockOn();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TraceRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TraceDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float DrawDebugTime;
	
private:
	void WaitInputPress();
	void FindTarget();

	void SetTimer_CheckDistance();
	void ClearTimer_CheckDistance();
	void CheckDistance();

	UFUNCTION()
	void OnInputPressed(float TimeWaited);

private:
	FTimerHandle CheckDistanceTimerHandle;
	
};


