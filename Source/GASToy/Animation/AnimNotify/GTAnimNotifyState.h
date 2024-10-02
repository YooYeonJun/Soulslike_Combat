// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "GTAnimNotifyState.generated.h"

/**
 * 애님 노티파이 베이스 클래스
 */
UCLASS(Abstract)
class GASTOY_API UGTAnimNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	FString GetNotifyName_Implementation() const override;
};
