// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GTLockOnTargetComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FGTLockOnEndedDelegate);

/**
 * 
 */
UCLASS(Blueprintable, meta=(BlueprintSpawnableComponent))
class GASTOY_API UGTLockOnTargetComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:
	UGTLockOnTargetComponent();

	FGTLockOnEndedDelegate LockOnEndedDelegate;

	UFUNCTION(BlueprintCallable)
	bool TryBeginLockOn();

	UFUNCTION(BlueprintCallable)
	bool TryEndLockOn();

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

private:
	void SetVisiblityAndChangeSpace(bool bInVisible);
	
	bool bIsLockedOn;
};
