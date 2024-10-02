// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "GTAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class GASTOY_API UGTAssetManager : public UAssetManager
{
	GENERATED_BODY()

public:
	static UGTAssetManager& Get();

	/** Starts initial load, gets called from InitializeObjectReferences */
	virtual void StartInitialLoading() override;
};
