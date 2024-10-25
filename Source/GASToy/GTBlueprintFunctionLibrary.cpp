// Copyright YYJ

#include "GTBlueprintFunctionLibrary.h"

EGTDirection UGTBlueprintFunctionLibrary::CalculateDirectionType(const float Degree)
{
	const float DegreeNormalized = FRotator::NormalizeAxis(Degree);
	const float DegreeNormalizedAbs = FMath::Abs(DegreeNormalized);
	const bool bIsPositive = DegreeNormalized > 0.0f;

	// // Debug
	// UE_LOG(LogTemp, Warning, TEXT("Degree : %f, Normalized : %f, Abs : %f"), Degree, DegreeNormalized, DegreeNormalizedAbs);
	
	if (DegreeNormalizedAbs <= 22.5f)
	{
		return EGTDirection::Forward;
	}
	if (DegreeNormalizedAbs <= 67.5f)
	{
		return bIsPositive ? EGTDirection::ForwardRight : EGTDirection::ForwardLeft;
	}
	if (DegreeNormalizedAbs <= 112.5f)
	{
		return bIsPositive ? EGTDirection::Right : EGTDirection::Left;
	}
	if (DegreeNormalizedAbs <= 157.5f)
	{
		return bIsPositive ? EGTDirection::BackwardRight : EGTDirection::BackwardLeft;
	}
	
	return EGTDirection::Backward;
}

FVector UGTBlueprintFunctionLibrary::CalculateDirectionVector(const EGTDirection Direction)
{
	static TMap<EGTDirection, FVector> DirectionToVectorMap =
		{
			{EGTDirection::None, FVector::ZeroVector},
			{EGTDirection::Forward, FVector::ForwardVector},
			{EGTDirection::ForwardLeft, (FVector::ForwardVector + FVector::LeftVector).GetUnsafeNormal()},
			{EGTDirection::ForwardRight, (FVector::ForwardVector + FVector::RightVector).GetUnsafeNormal()},
			{EGTDirection::Backward, FVector::BackwardVector},
			{EGTDirection::BackwardLeft, (FVector::BackwardVector + FVector::LeftVector).GetUnsafeNormal()},
			{EGTDirection::BackwardRight, (FVector::BackwardVector + FVector::RightVector).GetUnsafeNormal()},
			{EGTDirection::Left, FVector::LeftVector},
			{EGTDirection::Right, FVector::RightVector}
		};

	return DirectionToVectorMap[Direction];
}

