// Copyright YYJ


#include "GTAnimNotify_PlayCameraShake.h"

#include "Kismet/GameplayStatics.h"

UGTAnimNotify_PlayCameraShake::UGTAnimNotify_PlayCameraShake()
{
	InnerRadius = 0.0f;
	OuterRadius = 3000.0f;

	Falloff = 1.f;
	bOrientShakeTowardsEpicenter = false;
}

void UGTAnimNotify_PlayCameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
	const FAnimNotifyEventReference& EventReference)
{
	/** Plays an in-world camera shake that affects all nearby local players, with distance-based attenuation. Does not replicate.
	 * @param WorldContextObject - Object that we can obtain a world context from
	 * @param Shake - Camera shake asset to use
	 * @param Epicenter - location to place the effect in world space
	 * @param InnerRadius - Cameras inside this radius are ignored
	 * @param OuterRadius - Cameras outside of InnerRadius and inside this are effected
	 * @param Falloff - Affects falloff of effect as it nears OuterRadius
	 * @param bOrientShakeTowardsEpicenter - Changes the rotation of shake to point towards epicenter instead of forward
	 */

	check(MeshComp);
	UGameplayStatics::PlayWorldCameraShake(MeshComp, Shake, MeshComp->GetComponentLocation(), InnerRadius, OuterRadius,
		Falloff, bOrientShakeTowardsEpicenter);
}
