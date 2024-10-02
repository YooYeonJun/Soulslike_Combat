// Copyright YYJ


#include "GTBTService_RepeatIndex.h"

#include "BehaviorTree/BlackboardComponent.h"

UGTBTService_RepeatIndex::UGTBTService_RepeatIndex(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	INIT_SERVICE_NODE_NOTIFY_FLAGS();

	NodeName = "GTRepeatIndex";
	
	IndexBlackboardKey.AddIntFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, IndexBlackboardKey));
	
	Max = 1;
}

void UGTBTService_RepeatIndex::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (ensure(BBAsset))
	{
		IndexBlackboardKey.ResolveSelectedKey(*BBAsset);
	}
}

FString UGTBTService_RepeatIndex::GetStaticDescription() const
{
	return FString::Printf(TEXT("%s \nIndexBlackboardkey: %s \nMax: %d"),
		*Super::GetStaticDescription(), *IndexBlackboardKey.SelectedKeyName.ToString(), Max);
}

void UGTBTService_RepeatIndex::OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	RepeatIndex(OwnerComp);
}

void UGTBTService_RepeatIndex::RepeatIndex(UBehaviorTreeComponent& OwnerComp) const
{
	if (UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent())
	{
		int Index = BlackboardComp->GetValueAsInt(IndexBlackboardKey.SelectedKeyName);
		Index = (Index + 1) % (Max + 1);
		BlackboardComp->SetValueAsInt(IndexBlackboardKey.SelectedKeyName, Index);
	}
}
