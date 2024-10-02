// Copyright YYJ


#include "GTBTService_LockOn.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/AbilitySystem/Ability/LockOn/GTGameplayAbility_MonsterLockOn.h"
#include "GASToy/AI/GTBTFunctionLibrary.h"
#include "GASToy/Character/GTPlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"

UGTBTService_LockOn::UGTBTService_LockOn(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	NodeName = "GTLockOn";

	RangeRadius = 3000.0f;
	
	TargetActorBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetActorBlackboardKey), AActor::StaticClass());
}

void UGTBTService_LockOn::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetActorBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UGTBTService_LockOn::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nTargetActorBlackboardKey: %s \nRangeRadius: %f"),
		*Super::GetStaticDescription(), *TargetActorBlackboardKey.SelectedKeyName.ToString(), RangeRadius);
}

void UGTBTService_LockOn::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	LockOn(OwnerComp);
}

void UGTBTService_LockOn::LockOn(UBehaviorTreeComponent& OwnerComp)
{
	UGTAbilitySystemComponent* OwnerASC = UGTBTFunctionLibrary::GetGTAbilitySystemComponentFromOwnerComp(OwnerComp);
	if (!OwnerASC || OwnerASC->HasMatchingGameplayTag(GTGameplayTags::Character_State_IsLockOn))
	{
		return;
	}

	if (AAIController* AIOwner = OwnerComp.GetAIOwner())
	{
		if (APawn* OwnerPawn = AIOwner->GetPawn())
		{
			if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
			{
				if (AActor* Target = FindTarget(OwnerPawn))
				{
					const bool bIsActivated = UGTGameplayAbility_MonsterLockOn::TryActivate(OwnerPawn, Target);
					if (bIsActivated)
					{
						BlackboardComp->SetValueAsObject(TargetActorBlackboardKey.SelectedKeyName, Target);
					}
				}
				else
				{
					FGameplayTagContainer CancelTags{GTGameplayTags::Ability_State_LockOn};
					OwnerASC->CancelAbilities(&CancelTags);
					BlackboardComp->ClearValue(TargetActorBlackboardKey.SelectedKeyName);
				}
			}
		}
	}		
}

AActor* UGTBTService_LockOn::FindTarget(AActor* OwnerActor) const
{
	if (!OwnerActor)
	{
		return nullptr;
	}

	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes = {UCollisionProfile::Get()->ConvertToObjectType(ECC_Pawn)};
	const TArray<AActor*> ActorsToIgnore = {OwnerActor};
	TArray<AActor*> OverlappedActors;
	
	UKismetSystemLibrary::SphereOverlapActors(OwnerActor, OwnerActor->GetActorLocation(), RangeRadius,
		ObjectTypes, AGTPlayerCharacter::StaticClass(), ActorsToIgnore, OverlappedActors);

	return OverlappedActors.Num() > 0 ? OverlappedActors[0] : nullptr;
}
