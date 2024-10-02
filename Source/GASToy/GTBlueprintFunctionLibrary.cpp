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
	const UEnum* DirectionEnum = FindObjectChecked<UEnum>(nullptr, TEXT("/Script/GASToy.EGTDirection"), true);
	
	static TMap<EGTDirection, FVector> DirectionToVectorMap;
	DirectionToVectorMap.Reserve(DirectionEnum->NumEnums());
	DirectionToVectorMap.Add(EGTDirection::None, FVector::ZeroVector);
	DirectionToVectorMap.Add(EGTDirection::Forward, FVector::ForwardVector);
	DirectionToVectorMap.Add(EGTDirection::ForwardLeft, (FVector::ForwardVector + FVector::LeftVector).GetUnsafeNormal());
	DirectionToVectorMap.Add(EGTDirection::ForwardRight, (FVector::ForwardVector + FVector::RightVector).GetUnsafeNormal());
	DirectionToVectorMap.Add(EGTDirection::Backward, FVector::BackwardVector);
	DirectionToVectorMap.Add(EGTDirection::BackwardLeft, (FVector::BackwardVector + FVector::LeftVector).GetUnsafeNormal());
	DirectionToVectorMap.Add(EGTDirection::BackwardRight, (FVector::BackwardVector + FVector::RightVector).GetUnsafeNormal());
	DirectionToVectorMap.Add(EGTDirection::Left, FVector::LeftVector);
	DirectionToVectorMap.Add(EGTDirection::Right, FVector::RightVector);

	return DirectionToVectorMap[Direction];
}
