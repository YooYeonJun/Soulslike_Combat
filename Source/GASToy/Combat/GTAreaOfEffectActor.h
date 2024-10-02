// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GTCollisionSubsystem.h"
#include "GameFramework/Actor.h"
#include "GTAreaOfEffectActor.generated.h"

class USphereComponent;

/**
 * 스폰후 충돌검사를 통해 이벤트를 전달한다.
 * SphereComponent의 크기로 트레이스 검사한다.
 */
UCLASS()
class GASTOY_API AGTAreaOfEffectActor : public AActor
{
	GENERATED_BODY()

public:
	AGTAreaOfEffectActor();

	virtual void Tick(float DeltaSeconds) override;
	
	void Init(const FGTCollisionId& InCollisionId, const FGameplayTag& InEventTag);
	
	const FGameplayTag& GetEventTag() const;
	void SetEventTag(const FGameplayTag& InEventTag);

	void SetCollisionId(const FGTCollisionId& InCollisionId);
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGameplayTag EventTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bCheckCollisionOnTick;

private:
	void CheckCollisionAndSendEvent();

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> HitActors;
	
	FGTCollisionId CollisionId;
};
