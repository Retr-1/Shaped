#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "ShapedHUD.generated.h"

UCLASS()
class SHAPED_API AShapedHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void DrawHUD() override;

protected:
	void DrawStatusBar(const FVector2D& Position, const FVector2D& Size, float FillPercent, const FLinearColor& FillColor, const FString& Label);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor", meta = (ClampMin = "1.0"))
	float CursorHalfSize = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor", meta = (ClampMin = "0.0"))
	float CursorThickness = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor", meta = (ClampMin = "0.0"))
	float CursorGap = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
	FLinearColor CursorColor = FLinearColor::White;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars", meta = (ClampMin = "1.0"))
	float StatusBarWidth = 220.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars", meta = (ClampMin = "1.0"))
	float StatusBarHeight = 18.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars", meta = (ClampMin = "0.0"))
	float StatusBarPadding = 32.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars", meta = (ClampMin = "0.0"))
	float StatusBarSpacing = 12.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars", meta = (ClampMin = "0.0"))
	float StatusBarOutlineThickness = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars")
	FLinearColor StatusBarBackgroundColor = FLinearColor(0.02f, 0.02f, 0.02f, 0.75f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars")
	FLinearColor StatusBarOutlineColor = FLinearColor(0.85f, 0.85f, 0.85f, 0.9f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars")
	FLinearColor HealthBarColor = FLinearColor(0.78f, 0.14f, 0.14f, 0.95f);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Status Bars")
	FLinearColor StaminaBarColor = FLinearColor(0.18f, 0.72f, 0.30f, 0.95f);
};
