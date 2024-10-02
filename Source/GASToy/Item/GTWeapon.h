// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "GameFramework/Actor.h"
#include "GTWeapon.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class UCapsuleComponent;
class UNiagaraComponent;
class UGTCollisionCheckComponent;

/**
 * 캐릭터가 근접 공격을 할때 충돌검사를 하고 이벤트를 전달한다.
 * #### 현재 사용안함 ####
 */
UCLASS()
class GASTOY_API AGTWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	AGTWeapon();

	UFUNCTION(BlueprintCallable)
	void StartCollisionTest(const FGameplayTag& InEventTag);

	UFUNCTION(BlueprintCallable)
	void EndCollisionTest();

protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USceneComponent> RootSceneComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UNiagaraComponent> TrailNiagaraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UGTCollisionCheckComponent> CollisionCheckComponent;

private:
	void InitTrail() const;

private:
	FGameplayTag EventTag;
};
