// Copyright YYJ

#include "GTType.h"

namespace GTGameplayTags
{
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action, "Ability.Action");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Attack, "Ability.Action.Attack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Run, "Ability.Action.Run");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_GuardAndParry, "Ability.Action.GuardAndParry");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Dodge, "Ability.Action.Dodge");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Groggy, "Ability.Action.Groggy");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Execute, "Ability.Action.Execute");
	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_StandUp, "Ability.Action.StandUp");

	UE_DEFINE_GAMEPLAY_TAG(Ability_Action_Skill_A, "Ability.Action.Skill");

	UE_DEFINE_GAMEPLAY_TAG(Ability_State, "Ability.State");
	UE_DEFINE_GAMEPLAY_TAG(Ability_State_LockOn, "Ability.State.LockOn");
	UE_DEFINE_GAMEPLAY_TAG(Ability_State_ScanGroggy, "Ability.State.ScanGroggy");

	// HitReaction
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_KnockBack, "Ability.HitReaction.KnockBack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_Damaged, "Ability.HitReaction.Damaged");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_Airborne, "Ability.HitReaction.Airborne");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_TimeDilation, "Ability.HitReaction.TimeDilation");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_PlayMontage, "Ability.HitReaction.PlayMontage");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_ApplyRootMotion_KnockBack, "Ability.HitReaction.ApplyRootMotion.KnockBack");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_ApplyRootMotion_Airborne, "Ability.HitReaction.ApplyRootMotion.Airborne");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_PlayMontage_Damaged, "Ability.HitReaction.PlayMontage.Damaged");
	UE_DEFINE_GAMEPLAY_TAG(Ability_HitReaction_PlayMontage_Airborne, "Ability.HitReaction.PlayMontage.Airborne");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_State_InputBlocked, "Player.State.InputBlocked");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_InputBlocked_Move, "Player.State.InputBlocked.Move");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_InputBlocked_Look, "Player.State.InputBlocked.Look");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_InputSaving, "Player.State.InputSaving");

	UE_DEFINE_GAMEPLAY_TAG(Player_State_ActionInput_Saving, "Player.State.ActionInput.Saving");
	UE_DEFINE_GAMEPLAY_TAG(Player_State_ActionInput_CanReInput, "Player.State.ActionInput.CanReInput");

	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsRunning, "Character.State.IsRunning");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsLockOn, "Character.State.IsLockOn");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsGroggy, "Character.State.IsGroggy");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsExecuting, "Character.State.IsExecuting");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsAttacking, "Character.State.IsAttacking");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsHitReaction, "Character.State.IsHitReaction");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsKnockDown, "Character.State.IsKnockDown");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsKnockDown_Front, "Character.State.IsKnockDown.Front");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_IsKnockDown_Back, "Character.State.IsKnockDown.Back");
	
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Immunity_Damage, "Character.State.Immunity.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Immunity_HitReaction, "Character.State.Immunity.HitReaction");

	UE_DEFINE_GAMEPLAY_TAG(Character_State_Guard_CanGuard, "Character.State.Guard.CanGuard");
	UE_DEFINE_GAMEPLAY_TAG(Character_State_Guard_CanParry, "Character.State.Guard.CanParry");

	// BlockResult
	UE_DEFINE_GAMEPLAY_TAG(BlockingResult_Success, "BlockingResult.Success");
	UE_DEFINE_GAMEPLAY_TAG(BlockingResult_Success_Parry, "BlockingResult.Success.Parry");
	UE_DEFINE_GAMEPLAY_TAG(BlockingResult_Success_Guard, "BlockingResult.Success.Guard");
	UE_DEFINE_GAMEPLAY_TAG(BlockingResult_Failure, "BlockingResult.Failure");
	UE_DEFINE_GAMEPLAY_TAG(BlockingResult_Failure_Parry, "BlockingResult.Failure.Parry");
	UE_DEFINE_GAMEPLAY_TAG(BlockingResult_Failure_Guard, "BlockingResult.Failure.Guard");

	// Effect.SetByCaller
	UE_DEFINE_GAMEPLAY_TAG(Data_Damage, "Data.Damage");
	UE_DEFINE_GAMEPLAY_TAG(Data_Groggy, "Data.Groggy");

	// GameplayEvent
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Executed, "GameplayEvent.Executed");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_AttackEnd, "GameplayEvent.AttackEnd");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage, "GameplayEvent.Montage");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Execute, "GameplayEvent.Montage.Execute");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Attack_Dash, "GameplayEvent.Montage.Attack.Dash");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Attack_Slash_Light, "GameplayEvent.Montage.Attack.Slash.Light");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Attack_Slash_Mid, "GameplayEvent.Montage.Attack.Slash.Mid");
	UE_DEFINE_GAMEPLAY_TAG(GameplayEvent_Montage_Attack_Slash_Heavy, "GameplayEvent.Montage.Attack.Slash.Heavy");

	// GameplayCue
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Slash_BlockingFailure, "GameplayCue.Slash.BlockingFailure");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Slash_GuardSuccess, "GameplayCue.Slash.GuardSuccess");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_Slash_ParrySuccess, "GameplayCue.Slash.ParrySuccess");

	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_PhysicalAttack_BlockingFailure, "GameplayCue.PhysicalAttack.BlockingFailure");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_PhysicalAttack_GuardSuccess, "GameplayCue.PhysicalAttack.GuardSuccess");
	UE_DEFINE_GAMEPLAY_TAG(GameplayCue_PhysicalAttack_ParrySuccess, "GameplayCue.PhysicalAttack.ParrySuccess");
};