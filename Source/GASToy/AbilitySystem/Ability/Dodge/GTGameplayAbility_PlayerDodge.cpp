// Copyright YYJ


#include "GTGameplayAbility_PlayerDodge.h"

#include "AbilitySystemComponent.h"
#include "KismetAnimationLibrary.h"
#include "GASToy/GTBlueprintFunctionLibrary.h"
#include "GASToy/Character/GTPlayerCharacter.h"

void UGTGameplayAbility_PlayerDodge::CalculateMovementDirectionAndType(
	const FGameplayEventData* TriggerEventData, FVector& OutDirectionVector, EGTDirection& OutDirectionType) const
{
	OutDirectionVector = FVector::ZeroVector;
	OutDirectionType = EGTDirection::None;

	check(CurrentActorInfo);

	AGTPlayerCharacter* PlayerCharacter = CastChecked<AGTPlayerCharacter>(CurrentActorInfo->AvatarActor.Get());
	const FVector MovementInputVector = PlayerCharacter->GetMovementInputVector();

	const bool bIsMoving = MovementInputVector.IsNearlyZero() == false;
	if (bIsMoving)
	{
		const FRotator ControlRotation = PlayerCharacter->GetControlRotation();
		OutDirectionVector = MovementInputVector.RotateAngleAxis(ControlRotation.Yaw, FVector::UpVector);
		OutDirectionVector.Normalize();

		check(CurrentActorInfo->AbilitySystemComponent.Get());
		const bool bIsLockOn = CurrentActorInfo->AbilitySystemComponent->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsLockOn);
		if (bIsLockOn)
		{
			const float Degree = UKismetAnimationLibrary::CalculateDirection(MovementInputVector, FRotator::ZeroRotator);
			OutDirectionType = UGTBlueprintFunctionLibrary::CalculateDirectionType(Degree);
		}
		else
		{
			OutDirectionType = EGTDirection::Forward;
		}
	}
	else
	{
		OutDirectionType = EGTDirection::Backward;
		OutDirectionVector = PlayerCharacter->GetActorForwardVector().RotateAngleAxis(180.0f, FVector::UpVector);
	}
}

