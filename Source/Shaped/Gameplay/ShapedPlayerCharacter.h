#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapedPlayerCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player|Interaction")
	void StartDragging();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Player|Interaction")
	void StopDragging();

	UFUNCTION(BlueprintPure, Category = "Player")
	float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintPure, Category = "Player")
	float GetMaxHealth() const { return MaxHealth; }

	UFUNCTION(BlueprintPure, Category = "Player")
	UCameraComponent* GetFirstPersonCamera() const { return FirstPersonCamera; }

	UFUNCTION(BlueprintPure, Category = "Player")
	USkeletalMeshComponent* GetFirstPersonHandsMesh() const { return FirstPersonHandsMesh; }

	UFUNCTION(BlueprintPure, Category = "Player")
	EShapedGamePhase GetCurrentGamePhase() const;

	UFUNCTION(BlueprintPure, Category = "Player|Interaction")
	bool IsDragging() const { return bIsDragging; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|View")
	TObjectPtr<UCameraComponent> FirstPersonCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|View")
	TObjectPtr<USkeletalMeshComponent> FirstPersonHandsMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player", meta = (ClampMin = "1.0"))
	float MaxHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	float CurrentHealth = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player|Interaction")
	bool bIsDragging = false;

	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void FireCurrentAmmo_Implementation();
	virtual void Interact_Implementation();
	virtual void StartDragging_Implementation();
	virtual void StopDragging_Implementation();

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Value);
	void LookUpAtRate(float Value);
	void HandlePrimaryActionPressed();
	void HandlePrimaryActionReleased();
};
