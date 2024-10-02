// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "GTCollisionSubsystem.generated.h"


struct FGTCollisionId
{
public:
	bool operator==(const FGTCollisionId& Other) const
	{
		return Name == Other.Name && InstigatorWeak == Other.InstigatorWeak;
	}
	
	FName Name;
	TWeakObjectPtr<const AActor> InstigatorWeak;
};

struct FGTCollisionScope
{
public:
	bool operator==(const FGTCollisionScope& Other) const
	{
		return CollisionId == Other.CollisionId;
	}
	
	FGTCollisionId CollisionId;
	bool bHasSentEvent = false;
};

/**
 * 콜리젼 서브시스템
 * 중복 충돌 이벤트를 방지하기 위해 범위를 설정하고 범위내에 동일한 Id는 첫번째 이벤트만 발생한다.
 * 범위가 설정되있지 않으면, 항상 이벤트를 발생시킨다.
 */
UCLASS()
class GASTOY_API UGTCollisionSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	static UGTCollisionSubsystem* Get(const UObject* WorldContextObject);
	static UGTCollisionSubsystem* GetChecked(const UObject* WorldContextObject);

	void SendCollisionEventToTarget(AActor* Target, const FGameplayEventData& EventData, const FGTCollisionId& Id);
	
	void BeginScope(const FGTCollisionId& Id);
	void EndScope(const FGTCollisionId& Id);

private:
	bool HasSentEvent(const FGTCollisionId& Id) const;
	void MarkHasSentEvent(const FGTCollisionId& Id);

	FGTCollisionScope* FindScopeById(const FGTCollisionId& Id);
	const FGTCollisionScope* FindScopeById(const FGTCollisionId& Id) const;

private:
	TArray<FGTCollisionScope> Scopes;
};
