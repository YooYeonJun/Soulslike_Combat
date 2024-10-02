// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/Character/GTCharacter.h"
#include "GTMonsterCharacter.generated.h"

/**
 * 몬스터 캐릭터 클래스
 */
UCLASS()
class GASTOY_API AGTMonsterCharacter : public AGTCharacter
{
	GENERATED_BODY()

public:
	void Move(const FVector& WorldDirection);
};
