#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapedEnemyBase.generated.h"

class ABaseCore;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDefeatedSignature, AShapedEnemyBase*, Enemy);

UCLASS(Blueprintable)
class SHAPED_API AShapedEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AShapedEnemyBase();

	UPROPERTY(BlueprintAssignable, Category = "Enemy")
	FOnEnemyDefeatedSignature OnEnemyDefeated;

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	bool ApplyAmmoHit(FName AmmoId);

	UFUNCTION(BlueprintCallable, Category = "Enemy")
	void ReachBase(ABaseCore* BaseCore);

	UFUNCTION(BlueprintPure, Category = "Enemy")
	int32 GetResolvedHitCount() const { return ResolvedHitCount; }

	UFUNCTION(BlueprintPure, Category = "Enemy")
	const TArray<FName>& GetRequiredAmmoSequence() const { return AmmoRequirement.RequiredAmmoSequence; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy", meta = (ClampMin = "0.0"))
	float ContactDamage = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Enemy")
	FEnemyAmmoRequirement AmmoRequirement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Enemy")
	int32 ResolvedHitCount = 0;

	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnWrongAmmoHit(FName AmmoId);

	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnEnemyReachedBase();

	UFUNCTION(BlueprintImplementableEvent, Category = "Enemy")
	void OnEnemyDefeatedVisuals();

	void DefeatEnemy();
};
