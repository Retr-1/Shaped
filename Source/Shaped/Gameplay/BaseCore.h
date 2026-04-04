#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseCore.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnBaseDestroyedSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBaseHealthChangedSignature, float, NewHealth, float, MaxHealth);

UCLASS(Blueprintable)
class SHAPED_API ABaseCore : public AActor
{
	GENERATED_BODY()

public:
	ABaseCore();

	UPROPERTY(BlueprintAssignable, Category = "Base")
	FOnBaseDestroyedSignature OnBaseDestroyed;

	UPROPERTY(BlueprintAssignable, Category = "Base")
	FOnBaseHealthChangedSignature OnBaseHealthChanged;

	UFUNCTION(BlueprintCallable, Category = "Base")
	void ApplyBaseDamage(float DamageAmount);

	UFUNCTION(BlueprintPure, Category = "Base")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Base")
	float GetMaxHealth() const { return MaxHealth; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Base", meta = (ClampMin = "1.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Base")
	float CurrentHealth = 100.0f;

	virtual void BeginPlay() override;
};
