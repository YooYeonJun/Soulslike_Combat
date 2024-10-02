// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GTType.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GTBlueprintFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class GASTOY_API UGTBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "GTDirection")
	static EGTDirection CalculateDirectionType(const float Degree);

	UFUNCTION(BlueprintPure, Category = "GTDirection")
	static FVector CalculateDirectionVector(const EGTDirection Direction);
};
