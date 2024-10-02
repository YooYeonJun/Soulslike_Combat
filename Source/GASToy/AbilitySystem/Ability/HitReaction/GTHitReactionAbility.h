// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "GTHitReactionAbility.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class GASTOY_API UGTHitReactionAbility : public UGTGameplayAbility
{
	GENERATED_BODY()

public:
	UGTHitReactionAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
protected:
	template<typename HitReactionInfoType>
	static const HitReactionInfoType* GetHitReactionInfoFromEventDataChecked(const FGameplayEventData* EventData)
	{
		check(EventData);
		return CastChecked<HitReactionInfoType>(EventData->OptionalObject.Get());
	}

	AActor* GetAttacker() const { return AttackerWeak.Get(); }
	AActor* GetVictim() const { return VictimWeak.Get(); }

private:
	TWeakObjectPtr<AActor> AttackerWeak;
	TWeakObjectPtr<AActor> VictimWeak;
};
