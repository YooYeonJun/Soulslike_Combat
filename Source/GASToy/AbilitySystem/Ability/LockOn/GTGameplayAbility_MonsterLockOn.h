// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/LockOn/GTGameplayAbility_LockOn.h"
#include "GTGameplayAbility_MonsterLockOn.generated.h"

class AActor;

/**
 * 외부(감지 컴포넌트, 행동트리 등)에서 락온 대상을 찾고 어빌리티 실행시 이벤트 데이터로 넘어온 타깃을 락온한다.  
 */
UCLASS()
class GASTOY_API UGTGameplayAbility_MonsterLockOn : public UGTGameplayAbility_LockOn
{
	GENERATED_BODY()

public:
	UGTGameplayAbility_MonsterLockOn();

	static bool TryActivate(AActor* Instigator, const AActor* Target);

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

private:
	bool TryLockOnFromActor(const AActor* Actor);
};
