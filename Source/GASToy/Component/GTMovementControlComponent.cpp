// Copyright YYJ

#include "GTMovementControlComponent.h"

#include "AbilitySystemComponent.h"
#include "GASToy/Character/GTCharacter.h"
#include "GASToy/Character/GTPlayerCharacter.h"
#include "MotionWarpingComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GASToy/GTType.h"
#include "GASToy/AbilitySystem/Ability/LockOn/GTLockOnTargetComponent.h"
#include "Kismet/GameplayStatics.h"


UGTMovementControlComponent::UGTMovementControlComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	MovementMode = EGTMovementMode::None;

	Default_MaxMovementSpeed = 600.0f;
	Run_MaxMovementSpeed = 900.0f;
	LockOn_MaxMovementSpeed = 400.0f;

	LockOnYawMargin = 60.0f;
	LockOnPitchMargin = 70.0f;

	LockOnLookAtInterpSpeed = 8.0f;
	LockOnLookAtInterpSpeedInMargin = 0.1f;
}

void UGTMovementControlComponent::BeginPlay()
{
	Super::BeginPlay();

	AGTCharacter* OwnerCharacter = CastChecked<AGTCharacter>(GetOwner()); 
	OwnerCharacterWeak = OwnerCharacter;
	OwnerCharacterMovementComponentWeak = OwnerCharacter->GetCharacterMovement();
	OwnerASCWeak = OwnerCharacter->GetAbilitySystemComponent();
	OwnerMotionWarpingComponentWeak = OwnerCharacter->GetMotionWarpingComponent();
	OwnerSpringArmComponentWeak = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
}

void UGTMovementControlComponent::CalculateMovementMode()
{
	UAbilitySystemComponent* OwnerASC = OwnerASCWeak.Get();
	UCharacterMovementComponent* OwnerCharacterMovementComponent = OwnerCharacterMovementComponentWeak.Get();

	if (!OwnerASC || !OwnerCharacterMovementComponent)
	{
		return;
	}
	
	const bool bIsRunning = OwnerASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsRunning);
	const bool bIsLockOn = OwnerASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsLockOn);
	const bool bIsMoving = OwnerCharacterMovementComponent->Velocity.Size2D() >= 3.0f
		&& OwnerCharacterMovementComponent->GetCurrentAcceleration().IsNearlyZero() == false;

	if (GetCurrentMontage())
	{
		MovementMode = EGTMovementMode::Montage;
	}
	else if (bIsMoving && bIsRunning)
	{
		MovementMode = EGTMovementMode::Run;
	}
	else if (bIsLockOn)
	{
		MovementMode = EGTMovementMode::LockOn;
	}
	else if (bIsMoving)
	{
		MovementMode = EGTMovementMode::Walk;
	}
	else
	{
		MovementMode = EGTMovementMode::Idle;
	}

	if (MovementMode == EGTMovementMode::Run)
	{
		OwnerCharacterMovementComponent->MaxWalkSpeed = Run_MaxMovementSpeed;
		OwnerCharacterMovementComponent->bOrientRotationToMovement = true;
	}
	else if (MovementMode == EGTMovementMode::LockOn)
	{
		OwnerCharacterMovementComponent->MaxWalkSpeed = LockOn_MaxMovementSpeed;
		OwnerCharacterMovementComponent->bOrientRotationToMovement = false;
	}
	else
	{
		OwnerCharacterMovementComponent->MaxWalkSpeed = Default_MaxMovementSpeed;
		OwnerCharacterMovementComponent->bOrientRotationToMovement = true;
	}
}

void UGTMovementControlComponent::UpdateLookAtRotation()
{
	if (OwnerASCWeak->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsLockOn))
	{
		LookAtTarget_Camera();
	}

	if (GetMovementMode() == EGTMovementMode::LockOn && GetCurrentMontage() == nullptr)
	{
		LookAtTarget_Actor(LockOnLookAtInterpSpeed);
	}
}

void UGTMovementControlComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateMovementMode();
	UpdateLookAtRotation();
}

UAnimMontage* UGTMovementControlComponent::GetCurrentMontage() const
{
	if (const AGTCharacter* OwnerCharacter = OwnerCharacterWeak.Get())
	{
		return OwnerCharacter->GetCurrentMontage();
	}

	return nullptr;
}

void UGTMovementControlComponent::SetLockOnTargetComponent(UGTLockOnTargetComponent* InTargetComponent)
{
	LockOnTargetComponentWeak = InTargetComponent;

	static const FName TargetName = TEXT("Target");

	if (UMotionWarpingComponent* OwnerMotionWarpingComponent = OwnerMotionWarpingComponentWeak.Get())
	{
		if (const UGTLockOnTargetComponent* LockOnTargetComponent = LockOnTargetComponentWeak.Get())
		{
			if (ACharacter* TargetCharacter = Cast<ACharacter>(LockOnTargetComponent->GetOwner()))
			{
				OwnerMotionWarpingComponent->AddOrUpdateWarpTargetFromComponent(
						TargetName, TargetCharacter->GetMesh(), NAME_None, true);
			}			
		}
		else
		{
			OwnerMotionWarpingComponent->RemoveWarpTarget(TargetName);
		}
	}
}

EGTMovementMode UGTMovementControlComponent::GetMovementMode() const
{
	return MovementMode;
}

void UGTMovementControlComponent::LookAtTargetOrInputDirection(const float InterpSpeed)
{
	if (LockOnTargetComponentWeak.IsValid())
	{
		LookAtTarget_Actor(InterpSpeed);
	}
	else
	{
		LookAtInputDirection_Actor(InterpSpeed);
	}
}

void UGTMovementControlComponent::LookAtInputDirection_Actor(const float InterpSpeed)
{
	if (AGTPlayerCharacter* OwnerPlayerCharacter = Cast<AGTPlayerCharacter>(OwnerCharacterWeak.Get()))
	{
		const FVector MovementInputVector = OwnerPlayerCharacter->GetMovementInputVector();
		if (MovementInputVector.IsNearlyZero() == false)
		{
			const FVector ForwardVector = FRotator(0.0f, OwnerPlayerCharacter->GetControlRotation().Yaw, 0.0f).RotateVector(MovementInputVector);

			const FRotator CurrentRotation = OwnerPlayerCharacter->GetActorRotation();
			const FRotator TargetRotation = FRotationMatrix::MakeFromX(ForwardVector).Rotator();
			
			const FRotator Rotation = FMath::RInterpTo(CurrentRotation, TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed);
			OwnerPlayerCharacter->SetActorRotation(Rotation);
		}
	}
}

void UGTMovementControlComponent::LookAtTarget_Actor(const float InterpSpeed)
{
	const UGTLockOnTargetComponent* LockOnTargetComponent = LockOnTargetComponentWeak.Get();
	AGTCharacter* OwnerCharacter = OwnerCharacterWeak.Get();

	if (!LockOnTargetComponent || !OwnerCharacter)
	{
		return;
	}
	
	const FVector ForwardVector = LockOnTargetComponent->GetComponentLocation() - OwnerCharacter->GetActorLocation();

	const FRotator CurrentRotation = OwnerCharacter->GetActorRotation();
	const FRotator TargetRotation = FRotator(0.0f, FRotationMatrix::MakeFromX(ForwardVector).Rotator().Yaw, 0.0f);
			
	const FRotator Rotation = FMath::RInterpTo(CurrentRotation, TargetRotation, UGameplayStatics::GetWorldDeltaSeconds(this), InterpSpeed);
	OwnerCharacter->SetActorRotation(Rotation);
}

void UGTMovementControlComponent::LookAtTarget_Camera()
{
	const UGTLockOnTargetComponent* LockOnTargetComponent = LockOnTargetComponentWeak.Get();
	if (!LockOnTargetComponent)
	{
		return;
	}

	const USpringArmComponent* OwnerSpringArmComponent = OwnerSpringArmComponentWeak.Get();
	if (!OwnerSpringArmComponent)
	{
		return;
	}

	AGTCharacter* OwnerCharacter = OwnerCharacterWeak.Get();
	if (!OwnerCharacter)
	{
		return;
	}

	AController* OwnerController = OwnerCharacter->GetController();
	if (!OwnerController)
	{
		return;
	}

	const FVector TargetLocation = LockOnTargetComponent->GetComponentLocation();
	const FVector ToTargetVector = TargetLocation - OwnerSpringArmComponent->GetComponentLocation();

	const FRotator CurrentRotation = OwnerCharacter->GetControlRotation();

	const FRotator ToTargetRotation = FRotationMatrix::MakeFromX(ToTargetVector).Rotator();

	const float YawAlpha =
		FMath::Clamp(
			FMath::Abs(FRotator::NormalizeAxis(ToTargetRotation.Yaw - CurrentRotation.Yaw)) / LockOnYawMargin,
			0.0f, 1.0f);
	
	const float YawInterpSpeed = FMath::Lerp(LockOnLookAtInterpSpeedInMargin, LockOnLookAtInterpSpeed, YawAlpha);

	const float PitchAlpha =
		FMath::Clamp(
			FMath::Abs(FRotator::NormalizeAxis(ToTargetRotation.Pitch - CurrentRotation.Pitch)) / LockOnPitchMargin,
			0.0f, 1.0f);
	
	const float PitchInterpSpeed = FMath::Lerp(LockOnLookAtInterpSpeedInMargin, LockOnLookAtInterpSpeed, PitchAlpha);

	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);

	const FRotator YawRotation = FMath::RInterpTo(CurrentRotation, ToTargetRotation, DeltaTime, YawInterpSpeed);
	const FRotator PitchRotation = FMath::RInterpTo(CurrentRotation, ToTargetRotation, DeltaTime, PitchInterpSpeed);

	const FRotator Rotation = FRotator(PitchRotation.Pitch, YawRotation.Yaw, 0.0f);

	OwnerController->SetControlRotation(Rotation);
}

