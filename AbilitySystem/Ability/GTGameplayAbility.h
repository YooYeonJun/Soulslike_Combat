// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "GASToy/GTType.h"
#include "GASToy/AbilitySystem/Ability/GTAbilityInputDataAsset.h"
#include "HitReaction/GTHitReactionDesc.h"
#include "GTGameplayAbility.generated.h"

class UMotionWarpingComponent;
class UGTAbilitySystemComponent;
class APlayerController;
class UGameplayEffect;

USTRUCT(Blueprintable)
struct FGTDamageData
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Categories = "Character.State.Guard"))
	FGameplayTag IgnoreGuardTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Categories = "GameplayCue"))
	FGameplayTag BlockingFailureGameplayCueTag = GTGameplayTags::GameplayCue_Slash_BlockingFailure;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Categories = "GameplayCue"))
	FGameplayTag GuardSuccessGameplayCueTag = GTGameplayTags::GameplayCue_Slash_GuardSuccess;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Meta = (Categories = "GameplayCue"))
	FGameplayTag ParrySuccessGameplayCueTag = GTGameplayTags::GameplayCue_Slash_ParrySuccess;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> DamageEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Damage = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayEffect> GroggyEffectClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Groggy = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGTHitReactionDesc> HitReactionDescs;
};



/**
 * 
 */
UCLASS()
class GASTOY_API UGTGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()
	
public:
	UGTGameplayAbility();

	UFUNCTION(BlueprintPure)
	const FString& GetDescription() const { return Description; }

	UFUNCTION(BlueprintPure)
	EGTAbilityInputType GetInputType() const { return InputType; }

	UFUNCTION(BlueprintPure)
	UGTAbilitySystemComponent* GetGTAbilitySystemComponentFromActorInfo() const;

	UFUNCTION(BlueprintPure)
	APlayerController* GetPlayerControllerFromActorInfo() const;
	
	UFUNCTION(BlueprintPure)
	AActor* GetLockOnTarget() const;

	UFUNCTION(BlueprintPure)
	UMotionWarpingComponent* GetMotionWarpingComponentFromAvatarActor() const;

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;

	void InitAbilityTag(const FGameplayTag Tag, const bool bSetTrigger = false);

    UFUNCTION(BlueprintCallable)
    void ApplyDamage(const FGameplayEventData& EventData);

    UFUNCTION(BlueprintCallable)
    void WaitEvent_ApplyDamage(FGameplayTag EventTag, AActor* OptionalExternalTarget = nullptr, bool OnlyTriggerOnce = false, bool OnlyMatchExact = true);
    
    static FGameplayTag CheckBlocking(const FGameplayTag IgnoreGuardTag, const UAbilitySystemComponent* VictimASC);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString Description;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGTAbilityInputType InputType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bActivateAbilityOnGranted;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, FGTDamageData> EventTagToDamageData;

private:
	UFUNCTION()
	void OnEventReceived_ApplyDamage(FGameplayEventData EventData);
};
