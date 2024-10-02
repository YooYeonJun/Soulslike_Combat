// Copyright YYJ


#include "GASToy/Character/GTPlayerCharacter.h"
#include "GASToy/AbilitySystem/Ability/GTAbilityInputDataAsset.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/GTType.h"

#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"

DEFINE_LOG_CATEGORY(GTLogInput);

AGTPlayerCharacter::AGTPlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	SavedInputType = EGTAbilityInputType::None;
	MovementInputVector = FVector::ZeroVector;
}

void AGTPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();

	if (APlayerController* PC = GetController<APlayerController>())
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSubsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			check(DefaultInputMappingContext);
			InputSubsystem->ClearAllMappings();
			InputSubsystem->AddMappingContext(DefaultInputMappingContext, 0);
		}
	}
}

void AGTPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilitySystemComponent->RegisterGameplayTagEvent(
		GTGameplayTags::Player_State_ActionInput_Saving, EGameplayTagEventType::NewOrRemoved)
		.AddUObject(this, &ThisClass::OnInputSavingTagChanged);

	auto& Event = AbilitySystemComponent->OnAbilityEnded;
	if (!Event.IsBoundToObject(this))
	{
		Event.AddUObject(this, &ThisClass::OnAbilityEnded);
	}
}

void AGTPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	check(EnhancedInputComponent);

	check(MoveAction);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ThisClass::MoveCompleted);

	check(LookAction);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);

	check(AbilityInputDataAsset);
	for (const FGTAbilityInputBindInfo& BindInfo : AbilityInputDataAsset->GetBindInfos())
	{
		EnhancedInputComponent->BindAction(BindInfo.InputAction, ETriggerEvent::Triggered, this,
			&ThisClass::OnAbilityInputActionPressed, BindInfo.InputType);
		EnhancedInputComponent->BindAction(BindInfo.InputAction, ETriggerEvent::Completed, this,
			&ThisClass::OnAbilityInputActionCompleted, BindInfo.InputType);
	}
}

void AGTPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();
	MovementInputVector = FVector(MovementVector.Y, MovementVector.X, 0.0f);

	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(
		GTGameplayTags::Player_State_InputBlocked_Move))
	{
		return;
	}

	if (GetCurrentMontage() != nullptr)
	{
		return;
	}

	if (Controller)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGTPlayerCharacter::MoveCompleted(const FInputActionValue& Value)
{
	MovementInputVector = FVector::ZeroVector;
}

void AGTPlayerCharacter::Look(const FInputActionValue& Value)
{
	if (AbilitySystemComponent && AbilitySystemComponent->HasMatchingGameplayTag(
		GTGameplayTags::Player_State_InputBlocked_Look))
	{
		return;
	}

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGTPlayerCharacter::OnAbilityInputActionPressed(EGTAbilityInputType InputType)
{
	// 어빌리티 선입력 타입은 두가지 타입이 있다. (선입력 사용, 사용안함)
	// 어빌리티 입력 저장상태는 두가지 상태가 있다. (입력 저장중, 저장안함)
	// 선입력 사용 어빌리티는 저장상태가 저장중일땐 입력을 저장하고, 저장안함일땐 바로 실행 시도한다.
	// 저장에서 저장안함으로 바뀔때 저장한 입력을 시도한다.
	// 선입력 사용안하는 어빌리티는 입력이 들어오면 항상 시도한다.
	//
	// 예시)
	// 공격 A,B,C,D : 선입력 사용 / 가드, 락온 : 선입력 사용 안함
	// 시나리오1) 공격A 도중 공격B 입력함. / 3가지 경우가 있음
	// [----------------------Ability.Attack.A---------------------]
	// [-------------|-----입력저장중----------|---------------------]
	// [-------------IsAttacking-------------|---------------------]
	// [-----1번-----|---------2번------------|----------3번--------]
	// 공격B의 실행조건은 IsAttacking == false, Ability.Attack.A == true 이다.
	// 1번) 입력저장중이 아니므로 실행시도를 한다. 하지만 조건에 부합하지 않아서 시도 실패.
	// 2번) 입력저장중이므로 입력을 저장후, 저장상태가 끝났을때 실행시도를 한다. 이때 조건에 부합하므로 실행 성공.
	// 3번) 입력저장중이 아니므로 실행시도를 한다. 조건에 부합하므로 실행 성공.
	//
	// 시나리오2) 공격A 도중 가드를 입력함. / 2가지 경우가 있음
	// [----------------------Ability.Attack.A---------------------]
	// [-------------|-----입력저장중----------|---------------------]
	// [-------------IsAttacking-------------|---------------------]
	// [-----------------1번-----------------|----------2번---------]
	// 가드의 실행조건은 IsAttacking == false 이다.
	// 1번) 가드는 선입력을 사용안하므로 바로 시도한다. 하지만 실행조건에 부합하지않음.
	// 2번) 가드는 선입력을 사용안하므로 바로 시도한다. 조건에 부합해 실행 성공.
	//
	// 시나리오3) 공격A 도중 락온을 입력함. / 1가지 경우가 있음
	// [----------------------Ability.Attack.A---------------------]
	// [-------------|-----입력저장중----------|---------------------]
	// [-------------IsAttacking-------------|---------------------]
	// [---------------------------1번-----------------------------]
	// 락온의 실행조건은 없다.
	// 1번) 락온은 선입력을 사용안하므로 바로 시도한다. 조건없이 실행 성공.
	
	for (const FGTAbilityInputBindInfo& BindInfo : AbilityInputDataAsset->GetBindInfos())
	{
		if (BindInfo.InputType == InputType)
		{
			if (BindInfo.bPreInputSave)
			{
				if (AbilitySystemComponent->HasMatchingGameplayTag(GTGameplayTags::Player_State_ActionInput_Saving))
				{
					UE_LOG(GTLogInput, Warning,
						TEXT("YYJLog, AGTPlayerCharacter::OnAbilityInputActionPressed / PreInputSave, Saving [%s]"),
						*UEnum::GetValueAsString(SavedInputType));

					SavedInputType = InputType;
				}
				else
				{
					UE_LOG(GTLogInput, Warning,
						TEXT("YYJLog, AGTPlayerCharacter::OnAbilityInputActionPressed / PreInputSave, Try [%s]"),
						*UEnum::GetValueAsString(InputType));

					AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputType));
				}
			}
			else
			{
				UE_LOG(GTLogInput, Warning,
					TEXT("YYJLog, AGTPlayerCharacter::OnAbilityInputActionPressed / Not PreInputSave, Try [%s]"),
					*UEnum::GetValueAsString(InputType));

				AbilitySystemComponent->AbilityLocalInputPressed(static_cast<int32>(InputType));
			}

			return;
		} // if (BindInfo.InputType == InputType)
	} // for (const FGTAbilityInputBindInfo& BindInfo : AbilityInputDataAsset->GetBindInfos())
}

void AGTPlayerCharacter::OnAbilityInputActionCompleted(EGTAbilityInputType InputType)
{
	if (AbilitySystemComponent)
	{
		UE_LOG(GTLogInput, Warning,
			TEXT("YYJLog, AGTPlayerCharacter::OnAbilityInputActionCompleted / InputType [%s]"),
			*UEnum::GetValueAsString(InputType));

		AbilitySystemComponent->AbilityLocalInputReleased(static_cast<int32>(InputType));
	}
}

void AGTPlayerCharacter::OnInputSavingTagChanged(const FGameplayTag Tag, int32 Count)
{
	if (Count == 0)
	{
		if (SavedInputType != EGTAbilityInputType::None)
		{
			UE_LOG(GTLogInput, Warning,
				TEXT("YYJLog, AGTPlayerCharacter::OnInputSavingTagChanged / SavedInput Try [%s]"),
				*UEnum::GetValueAsString(SavedInputType));

			OnAbilityInputActionPressed(SavedInputType);
			SavedInputType = EGTAbilityInputType::None;
		}
	}
}

void AGTPlayerCharacter::OnAbilityEnded(const struct FAbilityEndedData& EndedData)
{
	UE_LOG(LogTemp, Warning, TEXT("YYJLog, AGTPlayerCharacter::OnAbilityEnded / "));
}
