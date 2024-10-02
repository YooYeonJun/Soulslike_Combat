// Copyright YYJ


#include "GTGameplayAbility_ScanGroggy.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"
#include "Abilities/Tasks/AbilityTask_WaitInputPress.h"
#include "GASToy/GTType.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"

UGTGameplayAbility_ScanGroggy::UGTGameplayAbility_ScanGroggy()
{
	InitAbilityTag(GTGameplayTags::Ability_State_ScanGroggy);

	InputType = EGTAbilityInputType::Attack;

	bActivateAbilityOnGranted = true;

	ScanRate = 0.1f;
	ScanRadius = 500.0f;
	ScanAngle = 90.0f;
	ScanObjectTypes.Add(UCollisionProfile::Get()->ConvertToObjectType(ECC_Pawn));
}

void UGTGameplayAbility_ScanGroggy::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	const FGameplayEventData* TriggerEventData)
{
	SetTimer_ScanGroggyActor();
	WaitInputPress();
	
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UGTGameplayAbility_ScanGroggy::EndAbility(const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
	bool bReplicateEndAbility, bool bWasCancelled)
{
	ClearTimer_ScanGroggyActor();
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UGTGameplayAbility_ScanGroggy::ScanGroggyActor()
{
	UAbilitySystemComponent* MyASC = GetAbilitySystemComponentFromActorInfo_Checked();
	if (MyASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsExecuting))
	{
		return;
	}

	GroggyActorWeak = nullptr;
	RemoveEffect_CanExecute();

	AActor* Avatar = GetAvatarActorFromActorInfo();
	if (!IsValid(Avatar))
	{
		return;
	}

	TArray<AActor*> TargetActors;
	TArray<AActor*> ActorsToIgnore {Avatar};
	UKismetSystemLibrary::SphereOverlapActors(this, Avatar->GetActorLocation(),
		ScanRadius, ScanObjectTypes, nullptr, ActorsToIgnore, TargetActors);

	for (AActor* TargetActor : TargetActors)
	{
		if (IsGroggyActor(TargetActor))
		{
			const FVector TargetToAvatarDirection = Avatar->GetActorLocation() - TargetActor->GetActorLocation();
			const float CurrentCosine = TargetToAvatarDirection.CosineAngle2D(TargetActor->GetActorForwardVector());
			const float ScanCosine = UKismetMathLibrary::DegCos(ScanAngle / 2.0f);

			// Debug
			// UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTGameplayAbility_ScanGroggy::ScanGroggyActor / "
			// 	"TargetActor[%s], CurrentCosine[%f], ScanCosine[%f]"), *GetNameSafe(TargetActor), CurrentCosine, ScanCosine);

			if (CurrentCosine >= ScanCosine)
			{
				GroggyActorWeak = TargetActor;
				ApplyEffect_CanExecute();

				break;
			}	
		}
	}
}

void UGTGameplayAbility_ScanGroggy::SetTimer_ScanGroggyActor()
{
	GetWorld()->GetTimerManager().SetTimer(ScanGroggyActorTimerHandle, FTimerDelegate::CreateUObject(this, &ThisClass::ScanGroggyActor),
		ScanRate, true);
}

void UGTGameplayAbility_ScanGroggy::ClearTimer_ScanGroggyActor()
{
	GetWorld()->GetTimerManager().ClearTimer(ScanGroggyActorTimerHandle);
}

void UGTGameplayAbility_ScanGroggy::ApplyEffect_CanExecute()
{
	CanExecuteEffectHandle = BP_ApplyGameplayEffectToOwner(CanExecuteEffectClass);
}

void UGTGameplayAbility_ScanGroggy::RemoveEffect_CanExecute()
{
	BP_RemoveGameplayEffectFromOwnerWithHandle(CanExecuteEffectHandle);
}

void UGTGameplayAbility_ScanGroggy::WaitInputPress()
{
	UAbilityTask_WaitInputPress* Task = UAbilityTask_WaitInputPress::WaitInputPress(this, false);

	Task->OnPress.AddUniqueDynamic(this, &ThisClass::OnInputPressed);
	Task->ReadyForActivation();
}

void UGTGameplayAbility_ScanGroggy::OnInputPressed(float TimeWaited)
{
	// 스캔시점과 입력시점이 차이가 있으므로 한번더 검사

	if (const AActor* GroggyActor = GroggyActorWeak.Get(); IsGroggyActor(GroggyActor))
	{
		FGameplayEventData EventData;

		AActor* OwningActor = GetOwningActorFromActorInfo();
		EventData.Instigator = OwningActor;
		EventData.Target = GroggyActor;
		EventData.EventTag = GTGameplayTags::Ability_Action_Execute;
		
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OwningActor, EventData.EventTag, EventData);		
	}

	WaitInputPress();
}

bool UGTGameplayAbility_ScanGroggy::IsGroggyActor(const AActor* Actor)
{
	if (!Actor)
	{
		return false;
	}

	UAbilitySystemComponent* ASC = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);
	if (ASC && ASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsGroggy))
	{
		return true;
	}

	return false;
}
