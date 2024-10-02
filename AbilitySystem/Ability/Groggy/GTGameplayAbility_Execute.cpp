// Copyright YYJ


#include "GTGameplayAbility_Execute.h"

#include "Abilities/Tasks/AbilityTask_ApplyRootMotionMoveToActorForce.h"
#include "GameFramework/RootMotionSource.h"
#include "GASToy/GTType.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"

UGTGameplayAbility_Execute::UGTGameplayAbility_Execute()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_Execute, true);
	
	TargetLocationOffset = FVector(100.0f, 0.0f, 0.0f);

	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_IsAttacking);
	ActivationBlockedTags.AddTag(GTGameplayTags::Character_State_IsGroggy);
}

void UGTGameplayAbility_Execute::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	check(TriggerEventData && TriggerEventData->Target);
	AActor* TargetActor = const_cast<AActor*>(TriggerEventData->Target.Get());
	check(TargetActor);
	
	ApplyEffect_IsExecuting();

	SetWarpTarget(TargetActor);
	
	SendExecutedEventToTarget(TargetActor);

	WaitEvent_ApplyDamage(GTGameplayTags::GameplayEvent_Montage_Execute);

	PlayMontageAndWait();

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_Execute::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffect_IsExecuting();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_Execute::ApplyEffect_IsExecuting()
{
	IsExecutingEffectHandle = BP_ApplyGameplayEffectToOwner(IsExecutingEffectClass);
}

void UGTGameplayAbility_Execute::RemoveEffect_IsExecuting()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(IsExecutingEffectHandle);
}

void UGTGameplayAbility_Execute::SetWarpTarget(const AActor* TargetActor) const
{
	check(TargetActor);
	
	if (UMotionWarpingComponent* MotionWarpingComponent = GetMotionWarpingComponentFromAvatarActor())
	{
		// TargetActor 앞으로 이동, 바라보게 회전
		static const FName ExecuteLocationName = TEXT("ExecuteLocation");
		const FVector WarpLocation = TargetActor->GetActorLocation() + TargetActor->GetActorRotation().RotateVector(TargetLocationOffset);
		const FRotator WarpRotation = TargetActor->GetActorRotation() + FRotator(0.0f, 180.0f, 0.0f);
		MotionWarpingComponent->AddOrUpdateWarpTargetFromLocationAndRotation(ExecuteLocationName, WarpLocation, WarpRotation);

		{
			// // Debug
			// const FVector LineStart = WarpLocation + FVector::UpVector * 50.0f;
			// const FVector LineEnd = LineStart + WarpRotation.Vector() * 100.0f;
			// DrawDebugDirectionalArrow(GetWorld(), LineStart, LineEnd, 50, FColor::Red,false, 1.0f,
			// 	0, 5);		
		}

	}
}

void UGTGameplayAbility_Execute::SendExecutedEventToTarget(AActor* TargetActor) const
{
	check(TargetActor);
	
	FGameplayEventData EventData;
	AActor* OwningActor = GetOwningActorFromActorInfo();
	EventData.Instigator = OwningActor;
	EventData.Target = TargetActor;
	EventData.EventTag = GTGameplayTags::GameplayEvent_Executed;
		
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(TargetActor, EventData.EventTag, EventData);
}

void UGTGameplayAbility_Execute::PlayMontageAndWait()
{
	UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, TEXT("Execute"), ExecuteMontage);

	Task->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnMontageEnded);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_Execute::OnMontageEnded()
{
	K2_EndAbility();
}
