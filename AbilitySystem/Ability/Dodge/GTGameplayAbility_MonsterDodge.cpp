// Copyright YYJ


#include "GTGameplayAbility_MonsterDodge.h"

#include "GASToy/GTBlueprintFunctionLibrary.h"

UGTGameplayAbility_MonsterDodge::UGTGameplayAbility_MonsterDodge()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_Dodge, true);
}

void UGTGameplayAbility_MonsterDodge::CalculateMovementDirectionAndType(
	const FGameplayEventData* TriggerEventData, FVector& OutDirectionVector, EGTDirection& OutDirectionType) const
{
	check(TriggerEventData);
	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!IsValid(Avatar))
	{
		return;
	}

	const UGTOptionalObject_MonsterDodge* MonsterDodgeData = CastChecked<UGTOptionalObject_MonsterDodge>(TriggerEventData->OptionalObject.Get());

	OutDirectionType = MonsterDodgeData->DodgeDirectionType;

	const FVector DirectionVector = UGTBlueprintFunctionLibrary::CalculateDirectionVector(MonsterDodgeData->DodgeDirectionType);
	OutDirectionVector = DirectionVector.RotateAngleAxis(Avatar->GetActorRotation().Yaw, FVector::UpVector); 
}
