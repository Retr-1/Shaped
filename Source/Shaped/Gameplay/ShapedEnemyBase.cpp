#include "Gameplay/ShapedEnemyBase.h"

#include "Gameplay/BaseCore.h"

AShapedEnemyBase::AShapedEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

bool AShapedEnemyBase::ApplyAmmoHit(FName AmmoId)
{
	if (!AmmoRequirement.RequiredAmmoSequence.IsValidIndex(ResolvedHitCount))
	{
		return false;
	}

	if (AmmoRequirement.RequiredAmmoSequence[ResolvedHitCount] != AmmoId)
	{
		OnWrongAmmoHit(AmmoId);
		return false;
	}

	++ResolvedHitCount;

	if (ResolvedHitCount >= AmmoRequirement.RequiredAmmoSequence.Num())
	{
		DefeatEnemy();
	}

	return true;
}

void AShapedEnemyBase::ReachBase(ABaseCore* BaseCore)
{
	if (BaseCore)
	{
		BaseCore->ApplyBaseDamage(ContactDamage);
	}

	OnEnemyReachedBase();
	Destroy();
}

void AShapedEnemyBase::DefeatEnemy()
{
	OnEnemyDefeated.Broadcast(this);
	OnEnemyDefeatedVisuals();
	Destroy();
}
