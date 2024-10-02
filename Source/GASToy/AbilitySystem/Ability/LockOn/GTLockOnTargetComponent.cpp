// Copyright YYJ

#include "GTLockOnTargetComponent.h"

UGTLockOnTargetComponent::UGTLockOnTargetComponent()
{
	DrawSize = FIntPoint(2,2);

	SetVisiblityAndChangeSpace(false);

	bIsLockedOn = false;
}

bool UGTLockOnTargetComponent::TryBeginLockOn()
{
	if (!bIsLockedOn)
	{
		bIsLockedOn = true;
		SetVisiblityAndChangeSpace(true);
		
		return true;
	}
	
	return false;
}

bool UGTLockOnTargetComponent::TryEndLockOn()
{
	if (bIsLockedOn)
	{
		bIsLockedOn = false;
		SetVisiblityAndChangeSpace(false);
		LockOnEndedDelegate.Broadcast();
		
		return true;
	}
	
	return false;
}

void UGTLockOnTargetComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	TryEndLockOn();
	
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UGTLockOnTargetComponent::SetVisiblityAndChangeSpace(bool bInVisible)
{
	SetVisibility(bInVisible);
	if (bInVisible)
	{
		SetWidgetSpace(EWidgetSpace::Screen);
	}
	else
	{
		// World로 해야 Trace검사가 된다.
		SetWidgetSpace(EWidgetSpace::World);
	}
}
