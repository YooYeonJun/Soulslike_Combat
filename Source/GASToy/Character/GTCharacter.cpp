// Copyright YYJ


#include "GASToy/Character/GTCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "GASToy/AbilitySystem/Attribute/GTAttributeSet.h"
#include "GASToy/AbilitySystem/GTAbilitySystemComponent.h"
#include "GASToy/AbilitySystem/Ability/GTGameplayAbility.h"
#include "MotionWarpingComponent.h"
#include "GASToy/Combat/GTCollisionSubsystem.h"
#include "GASToy/Component/GTCollisionCheckComponent.h"
#include "GASToy/Item/GTWeapon.h"

AGTCharacter::AGTCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AbilitySystemComponent = CreateDefaultSubobject<UGTAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UGTAttributeSet>(TEXT("AttributeSet"));

	bAddDefaultAbilties = false;
	bAddDefaultEffects = false;

	MotionWarpingComponent = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarpingComponent"));
	
	CollisionCheckComponent = CreateDefaultSubobject<UGTCollisionCheckComponent>(TEXT("CollisionCheckComponent"));
}

void AGTCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		
		HealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute()).AddUObject(this, &AGTCharacter::OnHealthChanged);
		MaxHealthChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute()).AddUObject(this, &AGTCharacter::OnMaxHealthChanged);
		HealthRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthRegenRateAttribute()).AddUObject(this, &AGTCharacter::OnHealthRegenRateChanged);
		ManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute()).AddUObject(this, &AGTCharacter::OnManaChanged);
		MaxManaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute()).AddUObject(this, &AGTCharacter::OnMaxManaChanged);
		ManaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaRegenRateAttribute()).AddUObject(this, &AGTCharacter::OnManaRegenRateChanged);
		StaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaAttribute()).AddUObject(this, &AGTCharacter::OnStaminaChanged);
		MaxStaminaChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxStaminaAttribute()).AddUObject(this, &AGTCharacter::OnMaxStaminaChanged);
		StaminaRegenRateChangedDelegateHandle = AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetStaminaRegenRateAttribute()).AddUObject(this, &AGTCharacter::OnStaminaRegenRateChanged);
	}

	if (HasAuthority())
	{
		AddDefaultAbilities();
		AddDefaultEffects();
		SpawnWeapon();
		InitCollisionCheckComponent();
	}
}

void AGTCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AGTCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

UAbilitySystemComponent* AGTCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AGTCharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	AbilitySystemComponent->GetOwnedGameplayTags(TagContainer);
}

bool AGTCharacter::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return AbilitySystemComponent->HasMatchingGameplayTag(TagToCheck);
}

bool AGTCharacter::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAllMatchingGameplayTags(TagContainer);
}

bool AGTCharacter::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return AbilitySystemComponent->HasAnyMatchingGameplayTags(TagContainer);
}

UGTAbilitySystemComponent* AGTCharacter::GetGTAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

UMotionWarpingComponent* AGTCharacter::GetMotionWarpingComponent() const
{
	return MotionWarpingComponent;
}

UGTAttributeSet* AGTCharacter::GetAttributeSet() const
{
	return AttributeSet;
}

bool AGTCharacter::IsAlive() const
{
	return GetHealth() > 0.0f;
}

float AGTCharacter::GetHealth() const
{
	return AttributeSet->GetHealth();
}

float AGTCharacter::GetMaxHealth() const
{
	return AttributeSet->GetMaxHealth();
}

float AGTCharacter::GetHealthRegenRate() const
{
	return AttributeSet->GetHealthRegenRate();
}

float AGTCharacter::GetMana() const
{
	return AttributeSet->GetMana();
}

float AGTCharacter::GetMaxMana() const
{
	return AttributeSet->GetMaxMana();
}

float AGTCharacter::GetManaRegenRate() const
{
	return AttributeSet->GetManaRegenRate();
}

float AGTCharacter::GetStamina() const
{
	return AttributeSet->GetStamina();
}

float AGTCharacter::GetMaxStamina() const
{
	return AttributeSet->GetMaxStamina();
}

float AGTCharacter::GetStaminaRegenRate() const
{
	return AttributeSet->GetStaminaRegenRate();
}

float AGTCharacter::GetArmor() const
{
	return AttributeSet->GetArmor();
}

float AGTCharacter::GetMoveSpeed() const
{
	return AttributeSet->GetMoveSpeed();
}

AGTWeapon* AGTCharacter::GetWeapon() const
{
	return Weapon;
}

void AGTCharacter::OnHealthChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnMaxHealthChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnHealthRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnManaChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnMaxManaChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnManaRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnStaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnMaxStaminaChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::OnStaminaRegenRateChanged(const FOnAttributeChangeData& Data)
{
}

void AGTCharacter::AddDefaultAbilities()
{
	if (!HasAuthority() || bAddDefaultAbilties)
	{
		return;
	}
	
	for (const TSubclassOf<UGTGameplayAbility>& Ability : DefaultAbilities)
	{
		AbilitySystemComponent->GiveAbility(
			FGameplayAbilitySpec(Ability, 1, static_cast<int32>(Ability.GetDefaultObject()->GetInputType()), this));
	}
}

void AGTCharacter::AddDefaultEffects()
{
	if (!HasAuthority() || bAddDefaultEffects)
	{
		return;
	}

	FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
	EffectContext.AddSourceObject(this);

	for (const TSubclassOf<UGameplayEffect>& Effect : DefaultEffects)
	{
		FGameplayEffectSpecHandle NewHandle = AbilitySystemComponent->MakeOutgoingSpec(Effect, 1, EffectContext);
		if (NewHandle.IsValid())
		{
			FActiveGameplayEffectHandle ActiveGEHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*NewHandle.Data.Get());
		}
	}
	
	bAddDefaultEffects = true;
}

void AGTCharacter::SpawnWeapon()
{
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	
	Weapon = GetWorld()->SpawnActor<AGTWeapon>(WeaponClass, SpawnParameters);
	if (!Weapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("YYJLog, AGTCharacter::SpawnWeapon / fail"));
		return;
	}

	static const FName WeaponSocketName = TEXT("Weapon_R");
    const bool bAttachSuccess = Weapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, WeaponSocketName);
	if (!bAttachSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("YYJLog, AGTCharacter::SpawnWeapon / Attach fail"));
		return;
	}
}

void AGTCharacter::InitCollisionCheckComponent()
{
	if (CollisionCheckComponent->CollisionCheckedDelegate.IsBoundToObject(this) == false)
	{
		CollisionCheckComponent->CollisionCheckedDelegate.BindWeakLambda(this, [this](const FGameplayTag& EventTag, AActor* HitActor, const FHitResult& HitResult)
		{
			FGameplayEventData EventData;
			EventData.EventTag = EventTag;
			EventData.Instigator = this;
			EventData.Target = HitActor;
			EventData.TargetData = UAbilitySystemBlueprintLibrary::AbilityTargetDataFromHitResult(HitResult);
			
			FGTCollisionId CollisionId;
			CollisionId.Name = TEXT("");
			CollisionId.InstigatorWeak = this;
			
			UGTCollisionSubsystem::GetChecked(this)->SendCollisionEventToTarget(this, EventData, CollisionId);
		});		
	}
}
