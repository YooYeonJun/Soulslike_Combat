// Copyright YYJ


#include "GTCollisionSubsystem.h"

#include "AbilitySystemBlueprintLibrary.h"

UGTCollisionSubsystem* UGTCollisionSubsystem::Get(const UObject* WorldContextObject)
{
	check(WorldContextObject);

	if (UWorld* World = WorldContextObject->GetWorld())
	{
		return World->GetSubsystem<UGTCollisionSubsystem>();
	}

	UE_LOG(LogTemp, Warning, TEXT("YYJLog, UGTCollisionSubsystem::Get / fail"));
	return nullptr;
}

UGTCollisionSubsystem* UGTCollisionSubsystem::GetChecked(const UObject* WorldContextObject)
{
	UGTCollisionSubsystem* Result = Get(WorldContextObject);
	check(Result);

	return Result;
}

void UGTCollisionSubsystem::SendCollisionEventToTarget(AActor* Target, const FGameplayEventData& EventData,
	const FGTCollisionId& Id)
{
	// Id가 같으면 충돌이벤트는 한번만 발생시킨다.
	if (HasSentEvent(Id))
	{
		return;
	}

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(Target, EventData.EventTag, EventData);
	MarkHasSentEvent(Id);
}

void UGTCollisionSubsystem::BeginScope(const FGTCollisionId& Id)
{
	if (FindScopeById(Id) != nullptr)
	{
		return;
	}

	FGTCollisionScope NewScope;
	NewScope.CollisionId = Id;
	NewScope.bHasSentEvent = false;

	Scopes.Add(NewScope);
}

void UGTCollisionSubsystem::EndScope(const FGTCollisionId& Id)
{
	for (int32 Index = 0; Index < Scopes.Num(); ++Index)
	{
		if (Scopes[Index].CollisionId == Id)
		{
			Scopes.RemoveAt(Index);
			return;
		}
	}
}

bool UGTCollisionSubsystem::HasSentEvent(const FGTCollisionId& Id) const
{
	if (const FGTCollisionScope* Scope = FindScopeById(Id))
	{
		return Scope->bHasSentEvent;
	}
	
	return false;
}

void UGTCollisionSubsystem::MarkHasSentEvent(const FGTCollisionId& Id)
{
	if (FGTCollisionScope* Scope = FindScopeById(Id))
	{
		Scope->bHasSentEvent = true;
	}
}

FGTCollisionScope* UGTCollisionSubsystem::FindScopeById(const FGTCollisionId& Id)
{
	return const_cast<FGTCollisionScope*>(AsConst(*this).FindScopeById(Id));
}

const FGTCollisionScope* UGTCollisionSubsystem::FindScopeById(const FGTCollisionId& Id) const
{
	return Scopes.FindByPredicate([Id](const FGTCollisionScope& Scope)
	{
		return Scope.CollisionId == Id;
	});
}
