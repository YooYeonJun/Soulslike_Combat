// Copyright YYJ


#include "GTHitReactionAbility_PlayMontage_Damaged.h"

UGTHitReactionAbility_PlayMontage_Damaged::UGTHitReactionAbility_PlayMontage_Damaged()
{
	InitAbilityTag(GTGameplayTags::Ability_HitReaction_PlayMontage_Damaged, true);
}

UAnimMontage* UGTHitReactionAbility_PlayMontage_Damaged::GetMontageAndUpdateVictimRotation()
{
	check(MontagesToPlay.Num() > 0);

	const int Index = FMath::RandRange(0, MontagesToPlay.Num() - 1);
	return MontagesToPlay[Index]; 
}
