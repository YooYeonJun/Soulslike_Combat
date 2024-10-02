// Copyright YYJ


#include "GTBTService_CalculateDistance.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Float.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "GASToy/AI/GTBTFunctionLibrary.h"

UGTBTService_CalculateDistance::UGTBTService_CalculateDistance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	NodeName = "GTCalculateDistance";

	TargetBlackboardKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetBlackboardKey), AActor::StaticClass());
	TargetBlackboardKey.AddVectorFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetBlackboardKey));
	
	DistanceBlackboardKey.AddFloatFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, DistanceBlackboardKey));
}

void UGTBTService_CalculateDistance::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		TargetBlackboardKey.ResolveSelectedKey(*BBAsset);
		DistanceBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UGTBTService_CalculateDistance::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nTargetBlackboardKey: %s \nDistanceBlackboardKey: %s"),
		*Super::GetStaticDescription(), *TargetBlackboardKey.SelectedKeyName.ToString(), *DistanceBlackboardKey.SelectedKeyName.ToString());
}

void UGTBTService_CalculateDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	CalculateDistance(OwnerComp);
}

void UGTBTService_CalculateDistance::CalculateDistance(UBehaviorTreeComponent& OwnerComp)
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		if (AAIController* AIOwner = OwnerComp.GetAIOwner())
		{
			if (APawn* OwnerPawn = AIOwner->GetPawn())
			{
				FVector TargetLocation;
				const bool bHasLocation = BlackboardComp->GetLocationFromEntry(TargetBlackboardKey.GetSelectedKeyID(), TargetLocation);
				if (bHasLocation)
				{
					const float Distance = FVector::Distance(OwnerPawn->GetActorLocation(), TargetLocation);
					BlackboardComp->SetValueAsFloat(DistanceBlackboardKey.SelectedKeyName, Distance);
				}				
			}
		}	
	}
}
