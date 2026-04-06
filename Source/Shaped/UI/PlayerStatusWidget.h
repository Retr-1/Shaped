#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"

class AShapedPlayerCharacter;
class UProgressBar;
class UTextBlock;

UCLASS()
class SHAPED_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = "Status")
	void BindToPlayer(AShapedPlayerCharacter* InPlayerCharacter);

protected:
	UPROPERTY(Transient)
	TObjectPtr<AShapedPlayerCharacter> BoundPlayerCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> HealthBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> StaminaBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> HealthLabel;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StaminaLabel;

	UFUNCTION()
	void HandleHealthChanged(float NewHealth, float MaxHealth);

	UFUNCTION()
	void HandleStaminaChanged(float NewStamina, float MaxStamina);

private:
	void UpdateHealthDisplay(float NewHealth, float MaxHealth);
	void UpdateStaminaDisplay(float NewStamina, float MaxStamina);
	void SetBarPercent(UProgressBar* ProgressBar, float CurrentValue, float MaxValue) const;
	void UnbindFromCurrentPlayer();
};
