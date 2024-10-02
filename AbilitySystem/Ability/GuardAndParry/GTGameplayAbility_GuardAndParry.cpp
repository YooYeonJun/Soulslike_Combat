// Copyright YYJ


#include "GTGameplayAbility_GuardAndParry.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEffectRemoved.h"
#include "Abilities/Tasks/AbilityTask_WaitGameplayEvent.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "Abilities/Tasks/AbilityTask_WaitInputRelease.h"
#include "GASToy/GTType.h"

// 가드와 패링
// [-----------------Ability.Action.GuardAndParry-----------------]
// [-----패링 가능----|------------------가드 가능-------------------]
//
//
// 애니메이션 : 가드 시작, 가드 중, 가드 종료, 가드 성공, 패링 성공
// 입력 : 누름, 해제, 다시누름(재입력)
// 
// 입력 누름
// [-------가드 시작------|---------가드 중(루프)-------------------]
// [----패링 가능----|-------------가드 가능------------------------]
//
// 입력 해제
// [---------------------------가드 종료--------------------------]
// [---------------------------가드 불가--------------------------]
//
// 입력과 애님은 싱크가 정확히 맞지 않는다.
// 입력을 우선 처리 후 애님을 맞춘다.
// [----누름--------|--패링 성공--|------------다시 누름------------]
// [---가드 시작-----|----패링 애님------|----가드 시작--------------]
// [---------------------------|오차발생|-------------------------]
// 상태 부여는 다시 눌렀을 때 바로한다.
// 애니메이션은 다시 눌렀을 때 성공 애님 재생 중 이라면 끝난 후 재생한다.
//
// 가드성공시 : 가드 성공 애님 재생, 완료 시 가드 중으로 변경
// 패링성공시 : 패링 성공 애님 재생, 완료 시 가드 중으로 변경
// 성공 후 재입력 시 : 패링 가능 상태 부여
// 성공 후 재입력 안 함 : 패링 가능 상태 부여 안함
//
// 입력 해제시
// 1) 가드중이고 패링 가능 상태일때 : 패링 가능 상태가 끝났을때 종료
// 2) 가드중이고 패링 가능 상태가 아닐때 : 바로 종료
// 3) 가드나 패링 성공 애님 재생중일때 : 애님 끝났을때 종료
//
// 종료시 공격을 막은적이 없다면 쿨다운 적용



UGTGameplayAbility_GuardAndParry::UGTGameplayAbility_GuardAndParry()
{
	InitAbilityTag(GTGameplayTags::Ability_Action_GuardAndParry);

	InputType = EGTAbilityInputType::Guard;

	bIsRightImpactOfLastHit = false;
	bIsUpImpactOfLastHit = false;
	bIsBlockingSuccess = false;
	bIsWaitToEndAbilty = false;

	OffsetForCheckUpImpact = 70.0f;
}

void UGTGameplayAbility_GuardAndParry::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{			
		EndAbility(Handle, ActorInfo, ActivationInfo, true, true);
		return;
	}

	Cleanup();

	PlayMontage_GuardStart();
	ApplyEffectAndWaitRemove_CanParry();
	ApplyEffect_CanGuard();
	
	// Bind events
	WaitEvent_BlockingResult();
	WaitInputRelease();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_GuardAndParry::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	RemoveEffect_CanParry();
	RemoveEffect_CanGuard();

	if (!bIsBlockingSuccess)
	{
		Super::ApplyCooldown(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo());
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_GuardAndParry::Cleanup()
{
	bIsBlockingSuccess = false;
	bIsWaitToEndAbilty = false;
}

void UGTGameplayAbility_GuardAndParry::PlayMontage_GuardStart()
{
	if (IsValid(MontageTask_GuardStartAndLoop))
	{
		MontageTask_GuardStartAndLoop->EndTask();
	}

	UAnimMontage* Montage = bIsRightImpactOfLastHit ? GuardStartAndLoopRightMontage : GuardStartAndLoopLeftMontage;
	MontageTask_GuardStartAndLoop = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, TEXT("GuardStart"), Montage);
	
	MontageTask_GuardStartAndLoop->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::PlayMontage_GuardLoop()
{
	if (IsValid(MontageTask_GuardStartAndLoop))
	{
		MontageTask_GuardStartAndLoop->EndTask();
	}
	
	UAnimMontage* Montage = bIsRightImpactOfLastHit ? GuardStartAndLoopRightMontage : GuardStartAndLoopLeftMontage;
	MontageTask_GuardStartAndLoop = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, TEXT("GuardLoop"), Montage, 1, TEXT("Loop"));
	
	MontageTask_GuardStartAndLoop->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::PlayMontage_GuardSuccess()
{
	if (IsValid(MontageTask_GuardSuccess))
	{
		MontageTask_GuardSuccess->EndTask();
	}
	
	UAnimMontage* Montage = bIsRightImpactOfLastHit ? GuardSuccessRightMontage : GuardSuccessLeftMontage;
	MontageTask_GuardSuccess = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, TEXT("GuardSuccess"), Montage);

	MontageTask_GuardSuccess->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_GuardSuccess->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_GuardSuccess->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_GuardSuccess->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_GuardSuccess->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::PlayMontage_ParrySuccess()
{
	if (IsValid(MontageTask_ParrySuccess))
	{
		MontageTask_ParrySuccess->EndTask();
	}
	
	UAnimMontage* Montage = bIsRightImpactOfLastHit ?
		(bIsUpImpactOfLastHit ? ParrySuccessRightUpMontage : ParrySuccessRightDownMontage) :
		(bIsUpImpactOfLastHit ? ParrySuccessLeftUpMontage : ParrySuccessLeftDownMontage);
		
	MontageTask_ParrySuccess = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(
	this, TEXT("ParrySuccess"), Montage);

	MontageTask_ParrySuccess->OnCompleted.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_ParrySuccess->OnBlendOut.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_ParrySuccess->OnInterrupted.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_ParrySuccess->OnCancelled.AddUniqueDynamic(this, &ThisClass::OnSuccessMontageEnded);
	MontageTask_ParrySuccess->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::OnSuccessMontageEnded()
{
	if (bIsWaitToEndAbilty)
	{
		K2_EndAbility();
	}
	else
	{
		PlayMontage_GuardLoop();
	}
}

void UGTGameplayAbility_GuardAndParry::ApplyEffectAndWaitRemove_CanParry()
{
	// 이펙트가 적용안돼있어야 한다.
	CanParryEffectHandle = BP_ApplyGameplayEffectToOwner(CanParryEffect);

	UAbilityTask_WaitGameplayEffectRemoved* Task = UAbilityTask_WaitGameplayEffectRemoved::WaitForGameplayEffectRemoved(
		this, CanParryEffectHandle);
	
	Task->OnRemoved.AddUniqueDynamic(this, &ThisClass::OnCanParryEffectRemoved);
	Task->ReadyForActivation();		
}

void UGTGameplayAbility_GuardAndParry::OnCanParryEffectRemoved(const FGameplayEffectRemovalInfo& GameplayEffectRemovalInfo)
{
	if (bIsWaitToEndAbilty)
	{
		const bool bCurrentMontageIsGuard =
			CurrentMontage == GuardStartAndLoopLeftMontage ||
			CurrentMontage == GuardStartAndLoopRightMontage;

		if (bCurrentMontageIsGuard)
		{
			K2_EndAbility();
		}
	}
}

void UGTGameplayAbility_GuardAndParry::RemoveEffect_CanParry()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(CanParryEffectHandle);
}

void UGTGameplayAbility_GuardAndParry::ApplyEffect_CanGuard()
{
	CanGuardEffectHandle = BP_ApplyGameplayEffectToOwner(CanGuardEffect);
}

void UGTGameplayAbility_GuardAndParry::RemoveEffect_CanGuard()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(CanGuardEffectHandle);
}

bool UGTGameplayAbility_GuardAndParry::IsRightImpactPoint(const FVector& ImpactPoint) const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();

	const FVector Forward = ImpactPoint - Avatar->GetActorLocation();
	return FVector::DotProduct(Forward, Avatar->GetActorRightVector()) > 0.0f;
}

bool UGTGameplayAbility_GuardAndParry::IsUpImpactPoint(const FVector& ImpactPoint) const
{
	const AActor* Avatar = GetAvatarActorFromActorInfo();
		
	const float Threshold = Avatar->GetActorLocation().Z + OffsetForCheckUpImpact;
	return ImpactPoint.Z > Threshold; 
}

void UGTGameplayAbility_GuardAndParry::ApplyGroggyEffectToAttacker(const FGameplayEventData& EventData) const
{
	UAbilitySystemComponent* AttackerASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(EventData.Instigator);
	FGameplayEffectSpecHandle GroggyEffectSpecHandle = MakeOutgoingGameplayEffectSpec(GroggyToAttackerEffect, GetAbilityLevel());
	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*GroggyEffectSpecHandle.Data, AttackerASC);
}

void UGTGameplayAbility_GuardAndParry::WaitEvent_BlockingResult()
{
	UAbilityTask_WaitGameplayEvent* Task = UAbilityTask_WaitGameplayEvent::WaitGameplayEvent(this, GTGameplayTags::BlockingResult_Success,
	nullptr, false, false);

	Task->EventReceived.AddUniqueDynamic(this, &ThisClass::OnEventReceived_BlockingResult);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::OnEventReceived_BlockingResult(FGameplayEventData EventData)
{
	bIsBlockingSuccess = true;
	
	if (const FGameplayAbilityTargetData* TargetData = EventData.TargetData.Get(0))
	{
		if (const FHitResult* HitResult = TargetData->GetHitResult())
		{
			//const FVector TestVector = HitResult->ImpactPoint;

			// TraceStart가 좀더 적절함.
			const FVector TestVector = HitResult->TraceStart;
			bIsRightImpactOfLastHit = IsRightImpactPoint(TestVector);
			bIsUpImpactOfLastHit = IsUpImpactPoint(TestVector);
		}
	}
	
	if (EventData.EventTag == GTGameplayTags::BlockingResult_Success_Parry)
	{
		ApplyGroggyEffectToAttacker(EventData);

		PlayMontage_ParrySuccess();
		
		RemoveEffect_CanParry();
	}
	else if (EventData.EventTag == GTGameplayTags::BlockingResult_Success_Guard)
	{
		PlayMontage_GuardSuccess();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTGameplayAbility_GuardAndParry::OnBlockingResultReceived / "
							   "EventTag[%s]"), *EventData.EventTag.ToString());
}

void UGTGameplayAbility_GuardAndParry::WaitInputPress()
{
	UAbilityTask_WaitInputPress* Task = UAbilityTask_WaitInputPress::WaitInputPress(this, false);
	Task->OnPress.AddUniqueDynamic(this, &ThisClass::OnInputPressed);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::OnInputPressed(float TimeWaited)
{
	WaitInputRelease();

	// 막기 성공한 상태일때만 재입력 가능
	if (bIsBlockingSuccess)
	{
		// ReGuard
		ApplyEffectAndWaitRemove_CanParry();
		bIsWaitToEndAbilty = false;
	}
}

void UGTGameplayAbility_GuardAndParry::WaitInputRelease()
{
	UAbilityTask_WaitInputRelease* Task = UAbilityTask_WaitInputRelease::WaitInputRelease(this, false);
	Task->OnRelease.AddUniqueDynamic(this, &ThisClass::OnInputReleased);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_GuardAndParry::OnInputReleased(float TimeHeld)
{
	WaitInputPress();
	
	bIsWaitToEndAbilty = false;
	bIsWaitToEndAbilty |= GetAbilitySystemComponentFromActorInfo()->HasMatchingGameplayTag(GTGameplayTags::Character_State_Guard_CanParry);

	bIsWaitToEndAbilty |=
		CurrentMontage == GuardSuccessLeftMontage ||
		CurrentMontage == GuardSuccessRightMontage ||
		CurrentMontage == ParrySuccessLeftUpMontage ||
		CurrentMontage == ParrySuccessLeftDownMontage ||
		CurrentMontage == ParrySuccessRightUpMontage ||
		CurrentMontage == ParrySuccessRightDownMontage;

	UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTGameplayAbility_GuardAndParry::OnInputRelease / CurrentMontage[%s], bIsWaitToEndAbility[%d]"),
		*GetNameSafe(CurrentMontage), bIsWaitToEndAbilty);
	
	if (!bIsWaitToEndAbilty)
	{
		K2_EndAbility();
	}
}



