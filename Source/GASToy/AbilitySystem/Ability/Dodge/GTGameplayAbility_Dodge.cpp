// Copyright YYJ


#include "GTGameplayAbility_Dodge.h"

#include "AbilitySystemComponent.h"
#include "Abilities/Tasks/AbilityTask_ApplyRootMotionConstantForce.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GASToy/GTType.h"
#include "GASToy/Component//GTMovementControlComponent.h"
#include "GASToy/Character/GTPlayerCharacter.h"

UGTGameplayAbility_Dodge::UGTGameplayAbility_Dodge()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_Dodge);
	InputType = EGTAbilityInputType::Dodge;
	
	RootMotionStrength = 800.0f;
	RootMotionDuration = 0.6f;
}

void UGTGameplayAbility_Dodge::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	FVector MovementDirectionVector = FVector::ZeroVector;
	EGTDirection MovementDirectionType = EGTDirection::None;
	CalculateMovementDirectionAndType(TriggerEventData, MovementDirectionVector, MovementDirectionType);

	LookAtTargetOrInputDirection();

	PlayMontageByDirectionType(MovementDirectionType);

	ApplyRootMotion(MovementDirectionVector);	

	ApplyEffect_IsDodging();
	ApplyEffect_DamageImmunity();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_Dodge::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffect_IsDodging();
	RemoveEffect_DamageImmunity();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_Dodge::LookAtTargetOrInputDirection()
{
	AActor* Avatar = GetAvatarActorFromActorInfo();
	check(Avatar);

	UGTMovementControlComponent* MovementControlComp = Avatar->GetComponentByClass<UGTMovementControlComponent>();
	check(MovementControlComp);

	MovementControlComp->LookAtTargetOrInputDirection(0.0f);
}

void UGTGameplayAbility_Dodge::PlayMontageByDirectionType(const EGTDirection DirectionType)
{
	UAnimMontage* MontageToPlay = DirectionTypeToMontage.FindChecked(DirectionType);

	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
		this, TEXT("Dodge"), MontageToPlay);

	Task->ReadyForActivation();
}

void UGTGameplayAbility_Dodge::ApplyEffect_IsDodging()
{
	IsDodgingEffectHandle = BP_ApplyGameplayEffectToOwner(IsDodgingEffectClass);
}

void UGTGameplayAbility_Dodge::RemoveEffect_IsDodging()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(IsDodgingEffectHandle);
}

void UGTGameplayAbility_Dodge::ApplyEffect_DamageImmunity()
{
	DamageImmunityEffectHandle = BP_ApplyGameplayEffectToOwner(DamageImmunityEffectClass);
}

void UGTGameplayAbility_Dodge::RemoveEffect_DamageImmunity()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(DamageImmunityEffectHandle);
}

void UGTGameplayAbility_Dodge::ApplyRootMotion(const FVector& Direction)
{
	UAbilityTask_ApplyRootMotionConstantForce* Task = UAbilityTask_ApplyRootMotionConstantForce::ApplyRootMotionConstantForce(
		this, TEXT("Dodge"), Direction, RootMotionStrength, RootMotionDuration, false, nullptr,
		ERootMotionFinishVelocityMode::SetVelocity, FVector::ZeroVector, false, true);

	Task->OnFinish.AddUniqueDynamic(this, &ThisClass::OnApplyRootMotionEnded);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_Dodge::OnApplyRootMotionEnded()
{
	K2_EndAbility();
}
