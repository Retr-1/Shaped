#include "Gameplay/BaseCore.h"

#include "Components/SceneComponent.h"

ABaseCore::ABaseCore()
{
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ABaseCore::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OnBaseHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void ABaseCore::ApplyBaseDamage(float DamageAmount)
{
	if (DamageAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
	OnBaseHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnBaseDestroyed.Broadcast();
	}
}
