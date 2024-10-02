// Copyright YYJ


#include "GTGroggyExecutionCalculation.h"

#include "GASToy/GTType.h"
#include "GASToy/AbilitySystem/Attribute/GTAttributeSet.h"

struct FGTGroggyStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Groggy);

	FGTGroggyStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UGTAttributeSet, Groggy, Target, true);
	}
};

static FGTGroggyStatics& GroggyStatics()
{
	static FGTGroggyStatics Statics;
	return Statics;
}

UGTGroggyExecutionCalculation::UGTGroggyExecutionCalculation()
{
	RelevantAttributesToCapture.Add(GroggyStatics().GroggyDef);
}

void UGTGroggyExecutionCalculation::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	
	float Groggy = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(FName("Data.Groggy")), false, 0.0f);
	
	if (TargetTags->HasTag(GTGameplayTags::Character_State_IsGroggy))
	{
		Groggy = 0.0f;
	}
	else if (TargetTags->HasTag(GTGameplayTags::BlockingResult_Success_Parry))
	{
		Groggy = 0.0f;
	}
	else if (TargetTags->HasTag(GTGameplayTags::BlockingResult_Success_Guard))
	{
		Groggy /= 2.0f;
	}
	
	const float GroggyDone = FMath::Max(0.0f, Groggy);
	
	if (GroggyDone > 0.0f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(UGTAttributeSet::GetGroggyAttribute(), EGameplayModOp::Additive, GroggyDone));
	}
}
