// Copyright YYJ


#include "GTAnimNotifyState_ApplyGameplayEffect.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

UGTAnimNotifyState_ApplyGameplayEffect::UGTAnimNotifyState_ApplyGameplayEffect()
{
}

void UGTAnimNotifyState_ApplyGameplayEffect::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromMeshOwner(MeshComp))
	{
		EffectHandle = OwnerASC->BP_ApplyGameplayEffectToSelf(EffectClass, 1.0f, FGameplayEffectContextHandle());
	}
}

void UGTAnimNotifyState_ApplyGameplayEffect::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	if (UAbilitySystemComponent* OwnerASC = GetAbilitySystemComponentFromMeshOwner(MeshComp))
	{
		OwnerASC->RemoveActiveGameplayEffect(EffectHandle);
	}
}

UAbilitySystemComponent* UGTAnimNotifyState_ApplyGameplayEffect::GetAbilitySystemComponentFromMeshOwner(
	const USkeletalMeshComponent* MeshComponent)
{
	check(MeshComponent);
	if (const AActor* OwnerActor = MeshComponent->GetOwner())
	{
		return UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(OwnerActor);
	}

	return nullptr;
}
