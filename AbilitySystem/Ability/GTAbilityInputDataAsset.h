// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GTAbilityInputDataAsset.generated.h"

class UInputAction;

UENUM(BlueprintType)
enum class EGTAbilityInputType : uint8
{
	None,
	Attack,
	Guard,
	Run,
	Dodge,
	LockOn,
};

USTRUCT(BlueprintType)
struct GASTOY_API FGTAbilityInputBindInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EGTAbilityInputType InputType = EGTAbilityInputType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UInputAction> InputAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bPreInputSave = false;
};

/**
 * 
 */
UCLASS()
class GASTOY_API UGTAbilityInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	const TArray<FGTAbilityInputBindInfo>& GetBindInfos() const { return BindInfos; }

private:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGTAbilityInputBindInfo> BindInfos;
};
