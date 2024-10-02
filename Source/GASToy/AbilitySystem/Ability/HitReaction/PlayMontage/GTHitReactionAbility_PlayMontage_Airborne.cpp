// Copyright YYJ


#include "GTHitReactionAbility_PlayMontage_Airborne.h"

UGTHitReactionAbility_PlayMontage_Airborne::UGTHitReactionAbility_PlayMontage_Airborne()
{
	InitAbilityTag(GTGameplayTags::Ability_HitReaction_PlayMontage_Airborne, true);
}

UAnimMontage* UGTHitReactionAbility_PlayMontage_Airborne::GetMontageAndUpdateVictimRotation()
{
	AActor* Attacker = GetAttacker();
	AActor* Victim = GetVictim();

	if (!Attacker || !Victim)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("YYJLog, UGTHitReactionAbility_PlayMontage_Airborne::GetMontageAndUpdateVictimRotation / fail,"
			"(!Attacker || !Victim)"));
		
		return nullptr;
	}
	
	const FVector VictimToAttacker = Attacker->GetActorLocation() - Victim->GetActorLocation();
	const FVector VictimForward = Victim->GetActorForwardVector();

	const bool bFrontHit = VictimForward.Dot(VictimToAttacker) > 0.0f;
	
	UAnimMontage* MontageToPlay = bFrontHit ? BackwardAirborneMontage : ForwardAirborneMontage;

	float Yaw = VictimToAttacker.Rotation().Yaw;
	if (!bFrontHit)
	{
		Yaw += 180.0f;
	}
	Victim->SetActorRotation(FRotator(0.0f, Yaw, 0.0f));
	
	return MontageToPlay;
}