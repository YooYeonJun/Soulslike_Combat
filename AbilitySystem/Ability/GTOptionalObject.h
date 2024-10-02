// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GTOptionalObject.generated.h"

/**
 * GameplayEventData.OptionalObject 로 사용하기 위한 베이스 클래스 
 */
UCLASS(Blueprintable, BlueprintType, Abstract, Const, DefaultToInstanced, EditInlineNew, CollapseCategories)
class GASTOY_API UGTOptionalObject : public UObject
{
	GENERATED_BODY()
	
};