// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GTCollisionCheckComponent.generated.h"

class UGTCollisionCheckComponent;
class UMeshComponent;
class AActor;

struct FGTCollisionCheckInstance
{
public:
	FVector Location = FVector::ZeroVector;
};

DECLARE_DELEGATE_ThreeParams(FGTCollisionCheckedDelegate, const FGameplayTag& /*EventTag*/, AActor* /*HitActor*/, const FHitResult& /*HitResult*/);

USTRUCT(BlueprintType)
struct FGTCollisionInfo
{
	GENERATED_BODY()

public:
	void Init(UGTCollisionCheckComponent* InOwnerComponent);

	void StartCollisionCheck(const FGameplayTag& InEventTag);
	void EndCollisionCheck();
	void CollisionCheck();

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName MeshComponentTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StartSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName EndSocketName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float CollisionRadius = 10.0f;

private:
	bool ShouldUpdate() const { return bShouldUpdate; }

	void CreateCollisionCheckInstances();
	void InitCollisionCheckInstances();
	void UpdateCollisionCheckInstances();

private:
	TWeakObjectPtr<UGTCollisionCheckComponent> OwnerComponentWeak;

	TWeakObjectPtr<UMeshComponent> MeshComponentWeak;
	
	UPROPERTY()
    TArray<TObjectPtr<AActor>> HitActors;

	FGameplayTag EventTag;

	TArray<FGTCollisionCheckInstance> CollisionCheckInstances;

	bool bShouldUpdate = false;

#pragma region Debug
public:
	void Debug_DrawCollisionInstances(UGTCollisionCheckComponent* InOwnerComp);

private:
	void Debug_Cleanup();
#pragma endregion
};

/**
 * 충돌처리 검사를 상세하게 하는 컴포넌트
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GASTOY_API UGTCollisionCheckComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGTCollisionCheckComponent();
	
	FGTCollisionCheckedDelegate CollisionCheckedDelegate;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UFUNCTION(BlueprintCallable)
	void StartCollisionCheck(const FName& Key, const FGameplayTag& EventTag);

	UFUNCTION(BlueprintCallable)
	void EndCollisionCheck(const FName& Key);
		
	const TArray<TEnumAsByte<EObjectTypeQuery>>& GetObjectTypes() const { return ObjectTypes; }
	
	bool ShouldDrawDebug() const { return bDrawDebug; }

	float GetDrawDebugTime() const { return DrawDebugTime; }
	
protected:
	virtual void BeginPlay() override;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TMap<FName, FGTCollisionInfo> KeyToCollisionInfo;

#pragma region Debug
protected:
	UFUNCTION(CallInEditor, Category = "Debug")
	void Debug_DrawCollisionInfo();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	FName KeyForDrawCollisionInfo;	
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	bool bDrawDebug;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Debug")
	float DrawDebugTime;
#pragma endregion
};
