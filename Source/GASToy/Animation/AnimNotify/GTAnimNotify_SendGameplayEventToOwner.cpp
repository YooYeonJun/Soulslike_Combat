// Copyright YYJ


#include "GTAnimNotify_SendGameplayEventToOwner.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UGTAnimNotify_SendGameplayEventToOwner::UGTAnimNotify_SendGameplayEventToOwner()
{
}

FString UGTAnimNotify_SendGameplayEventToOwner::GetNotifyName_Implementation() const
{
	if (EventTag.IsValid())
	{
		return EventTag.ToString();
	}
	else
	{
		return Super::GetNotifyName_Implementation();
	}
}

void UGTAnimNotify_SendGameplayEventToOwner::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	check(MeshComp);

	if (AActor* OwnerActor = MeshComp->GetOwner())
	{
		// OwnerASC가 있을때만 호출
		if (UAbilitySystemComponent* OwnerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor))
		{
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			EventData.Instigator = OwnerActor;
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwnerActor, EventTag, EventData);
		}		
	}
}
