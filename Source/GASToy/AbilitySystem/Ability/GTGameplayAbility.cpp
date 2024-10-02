// Copyright YYJ


#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/GTType.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "GameplayCueFunctionLibrary.h"
#include "MotionWarpingComponent.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "GASToy/Component/GTMovementControlComponent.h"

UGTGameplayAbility::UGTGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;

	InputType = EGTAbilityInputType::None;
	bActivateAbilityOnGranted = false;
}

UGTAbilitySystemComponent* UGTGameplayAbility::GetGTAbilitySystemComponentFromActorInfo() const
{
	return Cast<UGTAbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo());
}

APlayerController* UGTGameplayAbility::GetPlayerControllerFromActorInfo() const
{
	if (!ensure(CurrentActorInfo))
	{
		return nullptr;
	}
	return CurrentActorInfo->PlayerController.Get();
}

AActor* UGTGameplayAbility::GetLockOnTarget() const
{
	if (const AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		if (const UGTMovementControlComponent* MovementControlComponent = Avatar->FindComponentByClass<UGTMovementControlComponent>())
		{
			return MovementControlComponent->GetLockOnTarget();
		}
	}

	return nullptr;
}

UMotionWarpingComponent* UGTGameplayAbility::GetMotionWarpingComponentFromAvatarActor() const
{
	if (const AActor* Avatar = GetAvatarActorFromActorInfo())
	{
		UMotionWarpingComponent* Component = Avatar->FindComponentByClass<UMotionWarpingComponent>();
		check(Component);
		return Component;
	}

	return nullptr;
}

void UGTGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility::OnAvatarSet(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnAvatarSet(ActorInfo, Spec);

	if (bActivateAbilityOnGranted)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle, false);
	}
}

void UGTGameplayAbility::InitAbilityTag(const FGameplayTag Tag, const bool bSetTrigger)
{
	AbilityTags.AddTag(Tag);
	ActivationOwnedTags.AddTag(Tag);

	if (bSetTrigger)
	{
		FAbilityTriggerData TriggerData;
		TriggerData.TriggerTag = Tag;
		AbilityTriggers.Add(TriggerData);	
	}
}

void UGTGameplayAbility::ApplyDamage(const FGameplayEventData& EventData)
{
	// 데미지 적용
	// 1.막기검사
	// 2.막기결과 상태부여
	// 3.게임플레이 이펙트 처리 (데미지, 그로기)
	// 4.막기실패시 게임플레이큐 처리 (VFX, SFX)
	// 5.히트리액션, 가드와 패링처리
	// 6.막기결과 상태제거
	// 7.종료

	UAbilitySystemComponent* AttackerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Instigator);
	check(AttackerASC);
	
	UAbilitySystemComponent* VictimASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Target);
	check(VictimASC);

	const FGTDamageData& DamageData = EventTagToDamageData.FindChecked(EventData.EventTag);

	// 막기검사
	const FGameplayTag BlockingResultTag = CheckBlocking(DamageData.IgnoreGuardTag, VictimASC);

	// 막기결과 상태부여
	VictimASC->AddLooseGameplayTag(BlockingResultTag);
	{
		// 데미지 이펙트 처리
		FGameplayEffectSpecHandle DamageEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageData.DamageEffectClass, GetAbilityLevel());
		DamageEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), DamageData.Damage);
		AttackerASC->ApplyGameplayEffectSpecToTarget(*DamageEffectSpecHandle.Data, VictimASC);

		// 그로기 이펙트 처리
		FGameplayEffectSpecHandle GroggyEffectSpecHandle = MakeOutgoingGameplayEffectSpec(DamageData.GroggyEffectClass, GetAbilityLevel());
		GroggyEffectSpecHandle.Data.Get()->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Groggy")), DamageData.Groggy);
		AttackerASC->ApplyGameplayEffectSpecToTarget(*GroggyEffectSpecHandle.Data, VictimASC);

		// 게임플레이큐 (VFX, SFX) 처리
		if (!VictimASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_Immunity_Damage))
		{
			FGameplayTag GameplayCueTag;
            if (BlockingResultTag.MatchesTag(GTGameplayTags::BlockingResult_Failure))
            {
            	GameplayCueTag = DamageData.BlockingFailureGameplayCueTag;
            }
            else if (BlockingResultTag.MatchesTag(GTGameplayTags::BlockingResult_Success_Guard))
            {
            	GameplayCueTag = DamageData.GuardSuccessGameplayCueTag;
            }
            else if (BlockingResultTag.MatchesTag(GTGameplayTags::BlockingResult_Success_Parry))
            {
            	GameplayCueTag = DamageData.ParrySuccessGameplayCueTag;
            }
			
			if (GameplayCueTag.IsValid())
			{
				FGameplayEffectContextHandle EffectContextHandle = MakeEffectContext(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo());

				const FHitResult HitResult = UAbilitySystemBlueprintLibrary::GetHitResultFromTargetData(EventData.TargetData, 0);
				EffectContextHandle.AddHitResult(HitResult);

				const FGameplayCueParameters CueParams = {EffectContextHandle};

				AActor* Victim = const_cast<AActor*>(EventData.Target.Get());

				UGameplayCueFunctionLibrary::ExecuteGameplayCueOnActor(Victim, GameplayCueTag, CueParams);
			}
		}
		
		// 히트리액션처리
		for (const FGTHitReactionDesc& HitReactionDesc : DamageData.HitReactionDescs)
		{
			if (BlockingResultTag.MatchesAny(HitReactionDesc.BlockingResultRequiredTags))
			{
				AActor* Victim = const_cast<AActor*>(EventData.Target.Get());

				check(HitReactionDesc.Info);
				FGameplayEventData HitReactionEventData = EventData;
				HitReactionEventData.OptionalObject = HitReactionDesc.Info;
				UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Victim, HitReactionDesc.Info->ReactionTriggerTag,
					HitReactionEventData);
			}
		}

		// 가드와 패링 처리
		if (BlockingResultTag.MatchesTag(GTGameplayTags::BlockingResult_Success))
		{
			AActor* Victim = const_cast<AActor*>(EventData.Target.Get());
			UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Victim, BlockingResultTag, EventData);
		}
	}
	// 막기결과 상태제거
	VictimASC->RemoveLooseGameplayTag(BlockingResultTag);
}

void UGTGameplayAbility::WaitEvent_ApplyDamage(FGameplayTag EventTag, AActor* OptionalExternalTarget, bool OnlyTriggerOnce, bool OnlyMatchExact)
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(
	this, EventTag, OptionalExternalTarget, OnlyTriggerOnce, OnlyMatchExact);

	Task->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived_ApplyDamage);
	Task->ReadyForActivation();
}

void UGTGameplayAbility::OnEventReceived_ApplyDamage(FGameplayEventData EventData)
{
	ApplyDamage(EventData);
}

FGameplayTag UGTGameplayAbility::CheckBlocking(const FGameplayTag IgnoreGuardTag, const UAbilitySystemComponent* VictimASC)
{
	check(VictimASC);

	const bool bIgnoreParry = IgnoreGuardTag == GTGameplayTags::Character_State_Guard_CanParry;
	const bool bCanParry = VictimASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_Guard_CanParry);
	if (bCanParry)
	{
		if (bIgnoreParry)
		{
			return GTGameplayTags::BlockingResult_Failure_Parry;
		}
		else
		{
			return GTGameplayTags::BlockingResult_Success_Parry;
		}
	}

	const bool bIgnoreGuard = IgnoreGuardTag == GTGameplayTags::Character_State_Guard_CanGuard;
	const bool bCanGuard = VictimASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_Guard_CanGuard);
	if (bCanGuard)
	{
		if (bIgnoreParry || bIgnoreGuard)
		{
			return GTGameplayTags::BlockingResult_Failure_Guard;
		}
		else
		{
			return GTGameplayTags::BlockingResult_Success_Guard;
		}
	}

	return GTGameplayTags::BlockingResult_Failure;
}

