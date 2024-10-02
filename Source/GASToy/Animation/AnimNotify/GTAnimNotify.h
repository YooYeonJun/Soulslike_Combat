// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GTAnimNotify.generated.h"

/**
 * 애님 노티파이 베이스 클래스
 */
UCLASS(Abstract)
class GASTOY_API UGTAnimNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual FString GetNotifyName_Implementation() const override;
};
