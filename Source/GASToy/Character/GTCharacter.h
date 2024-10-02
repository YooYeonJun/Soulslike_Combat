// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayTagAssetInterface.h"
#include "GTCharacter.generated.h"

class UGTCollisionCheckComponent;
class AGTWeapon;
class UMotionWarpingComponent;
class UAbilitySystemComponent;
class UGTAbilitySystemComponent;
class UGTAttributeSet;
class UGTGameplayAbility;
class UGameplayEffect;

struct FOnAttributeChangeData;

/**
 * 기본 캐릭터 클래스
 */
UCLASS(Abstract)
class GASTOY_API AGTCharacter : public ACharacter, public IAbilitySystemInterface, public IGameplayTagAssetInterface
{
	GENERATED_BODY()

public:
	AGTCharacter();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	// ~IAbilitySystemInterface

	// IGameplayTagAssetInterface
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const override;
	virtual bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	virtual bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const override;
	// ~IGameplayTagAssetInterface
	
	UFUNCTION(BlueprintPure, Category = "Ability")
	UGTAbilitySystemComponent* GetGTAbilitySystemComponent() const;

	UFUNCTION(BlueprintPure)
	UMotionWarpingComponent* GetMotionWarpingComponent() const;

	UGTAttributeSet* GetAttributeSet() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	bool IsAlive() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetMaxHealth() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetHealthRegenRate() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetMana() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetMaxMana() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetManaRegenRate() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetStamina() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetMaxStamina() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetStaminaRegenRate() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetArmor() const;

	UFUNCTION(BlueprintPure, Category = "Attribute")
	float GetMoveSpeed() const;

	UFUNCTION(BlueprintPure)
	AGTWeapon* GetWeapon() const;

protected:
	virtual void BeginPlay() override;

	virtual void OnHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxHealthChanged(const FOnAttributeChangeData& Data);
	virtual void OnHealthRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void OnManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxManaChanged(const FOnAttributeChangeData& Data);
	virtual void OnManaRegenRateChanged(const FOnAttributeChangeData& Data);
	virtual void OnStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void OnMaxStaminaChanged(const FOnAttributeChangeData& Data);
	virtual void OnStaminaRegenRateChanged(const FOnAttributeChangeData& Data);

	virtual void AddDefaultAbilities();
	virtual void AddDefaultEffects();

	void SpawnWeapon();

	void InitCollisionCheckComponent();
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGTCollisionCheckComponent> CollisionCheckComponent;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UMotionWarpingComponent> MotionWarpingComponent;
	
	UPROPERTY()
	TObjectPtr<UGTAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UGTAttributeSet> AttributeSet;

	FDelegateHandle HealthChangedDelegateHandle;
	FDelegateHandle MaxHealthChangedDelegateHandle;
	FDelegateHandle HealthRegenRateChangedDelegateHandle;
	FDelegateHandle ManaChangedDelegateHandle;
	FDelegateHandle MaxManaChangedDelegateHandle;
	FDelegateHandle ManaRegenRateChangedDelegateHandle;
	FDelegateHandle StaminaChangedDelegateHandle;
	FDelegateHandle MaxStaminaChangedDelegateHandle;
	FDelegateHandle StaminaRegenRateChangedDelegateHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<UGTGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	bool bAddDefaultAbilties;
	bool bAddDefaultEffects;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AGTWeapon> WeaponClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<AGTWeapon> Weapon;
};
