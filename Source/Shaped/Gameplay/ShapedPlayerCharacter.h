#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapedPlayerCharacter.generated.h"

class UCameraComponent;
class USkeletalMeshComponent;
class AShapeObject;

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

	UFUNCTION(BlueprintPure, Category = "Player|Interaction")
	AShapeObject* GetHoveredShape() const { return CurrentHoveredShape; }

	UFUNCTION(BlueprintPure, Category = "Player|Interaction")
	AShapeObject* GetDraggedShape() const { return CurrentDraggedShape; }

	UFUNCTION(BlueprintPure, Category = "Player|Interaction")
	FVector GetCurrentDragTargetLocation() const;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction", meta = (ClampMin = "100.0"))
	float HoverTraceDistance = 1200.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction", meta = (ClampMin = "50.0"))
	float DragDistance = 350.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction", meta = (ClampMin = "50.0"))
	float MinDragDistance = 150.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction", meta = (ClampMin = "50.0"))
	float MaxDragDistance = 800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction", meta = (ClampMin = "1.0"))
	float DragScrollStep = 50.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player|Interaction", meta = (ClampMin = "1.0"))
	float DragForce = 2500.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Player|Interaction")
	TObjectPtr<AShapeObject> CurrentHoveredShape;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Player|Interaction")
	TObjectPtr<AShapeObject> CurrentDraggedShape;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
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
	void AdjustDragDistance(float Value);
	void IncreaseDragDistance();
	void DecreaseDragDistance();
	bool IsDraggedShapeOnScreen() const;
	void UpdateHoveredShape();
	void SetHoveredShape(AShapeObject* NewHoveredShape);
	void UpdateDraggedShape(float DeltaSeconds);
	void DrawDragDebugPoint() const;
};
