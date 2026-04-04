#include "Gameplay/ShapedPlayerCharacter.h"

AShapedPlayerCharacter::AShapedPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShapedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OnPlayerHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void AShapedPlayerCharacter::ApplyDamageToPlayer(float DamageAmount)
{
	if (DamageAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
	OnPlayerHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnPlayerDied.Broadcast();
	}
}

void AShapedPlayerCharacter::FireCurrentAmmo_Implementation()
{
}

void AShapedPlayerCharacter::Interact_Implementation()
{
}
