// Copyright YYJ


#include "GTDamageExecutionCalculation.h"

#include "GASToy/GTType.h"
#include "GASToy/AbilitySystem/Attribute/GTAttributeSet.h"

struct FGTDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Damage);

	FGTDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGTAttributeSet, Damage, Target, true);
	}
};

static FGTDamageStatics& DamageStatics()
{
	static FGTDamageStatics Statics;
	return Statics;
}

UGTDamageExecutionCalculation::UGTDamageExecutionCalculation()
{
	RelevantAttributesToCapture.Add(DamageStatics().DamageDef);
}

void UGTDamageExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
		
	float Damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Damage")), false, 0.0f);
	
	if (TargetTags->HasTag(GTGameplayTags::Character_State_Immunity_Damage))
	{
		Damage = 0.0f;
	}
	else if (TargetTags->HasTag(GTGameplayTags::BlockingResult_Success_Parry))
	{
		Damage = 0.0f;
	}
	else if (TargetTags->HasTag(GTGameplayTags::BlockingResult_Success_Guard))
	{
		Damage /= 2.0f;
	}
	
	const float DamageDone = FMath::Max(0.0f, Damage);
	
	if (DamageDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UGTAttributeSet::GetDamageAttribute(), EGameplayModOp::Additive, DamageDone));
	}
}
