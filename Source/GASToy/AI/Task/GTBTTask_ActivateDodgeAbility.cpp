// Copyright YYJ


#include "GTBTTask_ActivateDodgeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GASToy/AbilitySystem/Ability/Dodge/GTGameplayAbility_MonsterDodge.h"

UGTBTTask_ActivateDodgeAbility::UGTBTTask_ActivateDodgeAbility(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_TASK_NODE_NOTIFY_FLAGS();

	MonsterDodgeData = CreateDefaultSubobject<UGTOptionalObject_MonsterDodge>(TEXT("MonsterDodgeData"));
}

FString UGTBTTask_ActivateDodgeAbility::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nDirection: %s"),
		*Super::GetStaticDescription(),
		*UEnum::GetValueAsString(MonsterDodgeData->DodgeDirectionType));
}

FGameplayEventData UGTBTTask_ActivateDodgeAbility::CreateEventData() const
{
	FGameplayEventData EventData;
	EventData.EventTag = GetEventTag();
	EventData.OptionalObject = MonsterDodgeData;
	
	return EventData;
}

FGameplayTag UGTBTTask_ActivateDodgeAbility::GetEventTag() const
{
	return GTGameplayTags::Ability_Action_Dodge;
}