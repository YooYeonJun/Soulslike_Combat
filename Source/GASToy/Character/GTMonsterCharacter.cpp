// Copyright YYJ


#include "GASToy/Character/GTMonsterCharacter.h"

void AGTMonsterCharacter::Move(const FVector& WorldDirection)
{
	if (GetCurrentMontage() != nullptr)
	{
		return;
	}
	
	AddMovementInput(WorldDirection);
}
