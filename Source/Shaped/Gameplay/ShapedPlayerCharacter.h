#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShapedPlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerHealthChangedSignature, float, NewHealth, float, MaxHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerDiedSignature);

UCLASS(Blueprintable)
class SHAPED_API AShapedPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShapedPlayerCharacter();

	UPROPERTY(BlueprintAssignable, Category = "Player")
	FOnPlayerHealthChangedSignature OnPlayerHealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Player")
	FOnPlayerDiedSignature OnPlayerDied;

	UFUNCTION(BlueprintCallable, Category = "Player")
	void ApplyDamageToPlayer(float DamageAmount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void FireCurrentAmmo();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player")
	void Interact();

	UFUNCTION(BlueprintPure, Category = "Player")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Player")
	float GetMaxHealth() const { return MaxHealth; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player", meta = (ClampMin = "1.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	float CurrentHealth = 100.0f;

	virtual void BeginPlay() override;
	virtual void FireCurrentAmmo_Implementation();
	virtual void Interact_Implementation();
};
