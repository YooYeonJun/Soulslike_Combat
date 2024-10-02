// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/Character/GTCharacter.h"
#include "GameplayTags.h"
#include "GTPlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UGTAbilityInputDataAsset;

struct FInputActionValue;
enum class EGTAbilityInputType : uint8;

DECLARE_LOG_CATEGORY_EXTERN(GTLogInput, Log, All);

/**
 * 플레이어 캐릭터 클래스
 */
UCLASS()
class GASTOY_API AGTPlayerCharacter : public AGTCharacter
{
	GENERATED_BODY()
	
public:
	AGTPlayerCharacter();

	virtual void PawnClientRestart() override;

	USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure)
	FVector GetMovementInputVector() const { return MovementInputVector; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& Value);
	void MoveCompleted(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void OnAbilityInputActionPressed(EGTAbilityInputType InputType);
	void OnAbilityInputActionCompleted(EGTAbilityInputType InputType);

	void OnInputSavingTagChanged(const FGameplayTag Tag, int32 Count);
	void OnAbilityEnded(const struct FAbilityEndedData& EndedData);

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<const UGTAbilityInputDataAsset> AbilityInputDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> DefaultInputMappingContext;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> FollowCamera;

	UPROPERTY(BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	EGTAbilityInputType SavedInputType;

	FVector MovementInputVector;
};
