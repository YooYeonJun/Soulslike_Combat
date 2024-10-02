// Copyright YYJ

#include "GTGameplayAbility_LockOn.h"

#include "GTLockOnTargetComponent.h"
#include "GASToy/Component/GTMovementControlComponent.h"

UGTGameplayAbility_LockOn::UGTGameplayAbility_LockOn()
{
	InitAbilityTag(GTGameplayTags::Ability_State_LockOn);

	InputType = EGTAbilityInputType::LockOn;

	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_IsLockOn);
}

void UGTGameplayAbility_LockOn::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	TryEndLockOn();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

bool UGTGameplayAbility_LockOn::TryBeginLockOn(UGTLockOnTargetComponent* InTarget)
{
	if (!InTarget)
	{
		return false;
	}

	if (InTarget->TryBeginLockOn())
	{
		TargetWeak = InTarget;
		InTarget->LockOnEndedDelegate.AddWeakLambda(this, [this]()
		{
			OnLockOnEnded();
		});

		if (UGTMovementControlComponent* MovementControlComponent = GetOwnerMovementControlComponent())
		{
			MovementControlComponent->SetLockOnTargetComponent(InTarget);
		}

		ApplyEffect_IsLockOn();

		return true;
	}

	return false;
}

bool UGTGameplayAbility_LockOn::TryEndLockOn()
{
	if (UGTLockOnTargetComponent* Target = TargetWeak.Get())
	{
		return Target->TryEndLockOn();
	}
	
	return false;
}

UGTMovementControlComponent* UGTGameplayAbility_LockOn::GetOwnerMovementControlComponent() const
{
	if (const AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		return Avatar->FindComponentByClass<UGTMovementControlComponent>();
	}
	
	return nullptr;
}

void UGTGameplayAbility_LockOn::OnLockOnEnded()
{
	if (UGTMovementControlComponent* MovementControlComponent = GetOwnerMovementControlComponent())
	{
		MovementControlComponent->SetLockOnTargetComponent(nullptr);
	}

	RemoveEffect_IsLockOn();

	if (UGTLockOnTargetComponent* Target = TargetWeak.Get())
	{
		Target->LockOnEndedDelegate.RemoveAll(this);
		TargetWeak = nullptr;
	}
}

void UGTGameplayAbility_LockOn::ApplyEffect_IsLockOn()
{
	IsLockOnEffectHandle = BP_ApplyGameplayEffectToOwner(IsLockOnEffectClass);
}

void UGTGameplayAbility_LockOn::RemoveEffect_IsLockOn()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(IsLockOnEffectHandle);
}
