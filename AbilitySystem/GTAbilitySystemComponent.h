// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GTAbilitySystemComponent.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FGTOnAbilityEndedDelegate, const FGameplayTagContainer&, EndedTags);

/**
 * 
 */
UCLASS()
class GASTOY_API UGTAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()
	
public:
	UGTAbilitySystemComponent(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintAssignable)
	FGTOnAbilityEndedDelegate OnAbilityEndedDelegate;
		
	UFUNCTION(BlueprintPure)
	static UGTAbilitySystemComponent* GetGTAbilitySystemComponentFromActor(const AActor* Actor, bool LookForComponent = true);

	UFUNCTION(BlueprintCallable)
	void CancelAbilitiesByTags(const FGameplayTagContainer& WithTags, const FGameplayTagContainer& WithoutTags);

	UFUNCTION(BlueprintCallable)
	void CancelAbilityByTag(const FGameplayTag Tag);

	UFUNCTION(BlueprintPure)
	bool IsActiveAbility(UGameplayAbility* Ability) const;

	UFUNCTION(BlueprintPure)
	bool IsActiveAbilityByTags(const FGameplayTagContainer& Tags) const;
};
