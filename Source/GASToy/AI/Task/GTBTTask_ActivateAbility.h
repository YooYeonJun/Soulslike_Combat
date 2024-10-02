// Copyright YYJ

#pragma once

#include "CoreMinimal.h"
#include "GASToy/AI/Task/GTBTTaskNode.h"
#include "GTBTTask_ActivateAbility.generated.h"

class UGTAbilitySystemComponent;
class UGameplayAbility;
/**
 * 어빌리티를 실행하기위한 베이스 클래스
 */
UCLASS(Abstract)
class GASTOY_API UGTBTTask_ActivateAbility : public UGTBTTaskNode
{
	GENERATED_BODY()

public:
	UGTBTTask_ActivateAbility(const FObjectInitializer& ObjectInitializer);

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual bool IsActiveAbility() PURE_VIRTUAL(UGTBTTask_ActivateAbility::IsActiveAbility, return false; );
	virtual bool TryActivateAbility() PURE_VIRTUAL(UGTBTTask_ActivateAbility::TryActivateAbility, return false; );
	virtual void CancelAbility() PURE_VIRTUAL(UGTBTTask_ActivateAbility::CancelAbility, );
	virtual bool IsMyAbility(UGameplayAbility* Ability) PURE_VIRTUAL(UGTBTTask_ActivateAbility::IsMyAbility, return false; );

	UGTAbilitySystemComponent* GetOwnerGTAbilitySystemComponent() const { return OwnerASCWeak.Get(); }
	
protected:
	UPROPERTY(EditAnywhere)
	bool bWaitForEndAbility;

	UPROPERTY(EditAnywhere)
	bool bSuccessIfAbilityIsActive;
	
private:
	void OnAbilityEnded(UGameplayAbility* Ability);

	TWeakObjectPtr<UBehaviorTreeComponent> OwnerCompWeak;
	TWeakObjectPtr<UGTAbilitySystemComponent> OwnerASCWeak;
};
