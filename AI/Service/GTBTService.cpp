// Copyright YYJ


#include "GTBTService.h"

#include "AbilitySystemGlobals.h"
#include "AIController.h"

UGTBTService::UGTBTService(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Interval = 0.1f;
	RandomDeviation = 0.0f;
}
