// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GameplayTags.h"
#include "NativeGameplayTags.h"

namespace GTGameplayTags
{
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_Attack);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_Run);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_Dodge);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_GuardAndParry);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_Groggy);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_Execute);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_StandUp);

	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_Action_Skill);
	
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_LockOn);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_State_ScanGroggy);

	// HitReaction
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_KnockBack);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_Damaged);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_Airborne);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_TimeDilation);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_ApplyRootMotion_KnockBack);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_ApplyRootMotion_Airborne);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_PlayMontage_Damaged);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Ability_HitReaction_PlayMontage_Airborne);
	
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_InputBlocked);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_InputBlocked_Move);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_InputBlocked_Look);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_InputSaving);

	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_ActionInput_Saving);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_State_ActionInput_CanReInput);

	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsRunning);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsLockOn);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsGroggy);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsExecuting);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsAttacking);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsHitReaction);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsKnockDown);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsKnockDown_Front);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_IsKnockDown_Back);
	
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Immunity_Damage);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Immunity_HitReaction);

	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Guard_CanGuard);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Character_State_Guard_CanParry);

	// BlockResult
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockingResult_Success);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockingResult_Success_Parry);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockingResult_Success_Guard);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockingResult_Failure);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockingResult_Failure_Parry);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(BlockingResult_Failure_Guard);

	// Effect.SetByCaller
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Damage);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Data_Groggy);

	// GameplayEvent
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Executed);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_AttackEnd);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Execute);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Attack_Dash);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Attack_Slash_Light);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Attack_Slash_Mid);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayEvent_Montage_Attack_Slash_Heavy);

	// GameplayCue
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Slash_BlockingFailure);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Slash_GuardSuccess);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_Slash_ParrySuccess);

	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_PhysicalAttack_BlockingFailure);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_PhysicalAttack_GuardSuccess);
	GASTOY_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(GameplayCue_PhysicalAttack_ParrySuccess);
};

UENUM(BlueprintType)
enum class EGTDirection : uint8
{
	None,
	Left,
	Right,
	Forward,
	ForwardLeft,
	ForwardRight,
	Backward,
	BackwardLeft,
	BackwardRight
};