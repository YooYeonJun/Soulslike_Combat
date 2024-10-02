// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GASToy/AbilitySystem/Ability/LockOn/GTLockOnTargetComponent.h"
#include "GTMovementControlComponent.generated.h"

class USpringArmComponent;
class UMotionWarpingComponent;
class AGTCharacter;
class UCharacterMovementComponent;
class UAbilitySystemComponent;
class UGTLockOnTargetComponent;

UENUM(BlueprintType)
enum class EGTMovementMode : uint8
{
	None,
	Montage,
	Idle,
	Walk,
	Run,
	LockOn
};

/**
 * 캐릭터의 이동상태 (걷기, 달리기, 락온 등) 를 파악하여 최대 이동속도, 캐릭터 회전등을 처리한다.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASTOY_API UGTMovementControlComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UGTMovementControlComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintPure)
	float GetDefault_MaxMovementSpeed() const { return Default_MaxMovementSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetDefault_MaxMovementSpeed(const float MaxMovementSpeed) { Default_MaxMovementSpeed = MaxMovementSpeed; }
	
	UFUNCTION(BlueprintPure)
	float GetRun_MaxMovementSpeed() const { return Run_MaxMovementSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetRun_MaxMovementSpeed(const float MaxMovementSpeed) { Run_MaxMovementSpeed = MaxMovementSpeed; }
	
	UFUNCTION(BlueprintPure)
	float GetLockOn_MaxMovementSpeed() const { return LockOn_MaxMovementSpeed; }
	
	UFUNCTION(BlueprintCallable)
	void SetLockOn_MaxMovementSpeed(const float MaxMovementSpeed) { LockOn_MaxMovementSpeed = MaxMovementSpeed; }
	
	UFUNCTION(BlueprintPure)
	UAnimMontage* GetCurrentMontage() const;

	UFUNCTION(BlueprintPure)
	EGTMovementMode GetMovementMode() const;

	UFUNCTION(BlueprintPure)
	AActor* GetLockOnTarget() const { return LockOnTargetComponentWeak->GetOwner(); }

	UFUNCTION(BlueprintPure)
	const UGTLockOnTargetComponent* GetLockOnTargetComponent() const { return LockOnTargetComponentWeak.Get(); }
	
	UFUNCTION(BlueprintCallable)
	void SetLockOnTargetComponent(UGTLockOnTargetComponent* InTargetComponent);

	UFUNCTION(BlueprintCallable)
	void LookAtTargetOrInputDirection(const float InterpSpeed);

	UFUNCTION(BlueprintCallable)
	void LookAtInputDirection_Actor(const float InterpSpeed);

	UFUNCTION(BlueprintCallable)
	void LookAtTarget_Actor(const float InterpSpeed);

	UFUNCTION(BlueprintCallable)
	void LookAtTarget_Camera();
	
protected:
	virtual void BeginPlay() override;

	void CalculateMovementMode();
	void UpdateLookAtRotation();

protected:
	TWeakObjectPtr<AGTCharacter> OwnerCharacterWeak;
	
	TWeakObjectPtr<UAbilitySystemComponent> OwnerASCWeak;

	TWeakObjectPtr<UCharacterMovementComponent> OwnerCharacterMovementComponentWeak;

	TWeakObjectPtr<UMotionWarpingComponent> OwnerMotionWarpingComponentWeak;

	TWeakObjectPtr<const UGTLockOnTargetComponent> LockOnTargetComponentWeak;

	TWeakObjectPtr<const USpringArmComponent> OwnerSpringArmComponentWeak;

	EGTMovementMode MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Default_MaxMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Run_MaxMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOn_MaxMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnYawMargin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnPitchMargin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnLookAtInterpSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float LockOnLookAtInterpSpeedInMargin;
};
