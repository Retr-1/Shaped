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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor", meta = (ClampMin = "1.0"))
	float CursorHalfSize = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor", meta = (ClampMin = "0.0"))
	float CursorThickness = 1.5f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor", meta = (ClampMin = "0.0"))
	float CursorGap = 4.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Cursor")
	FLinearColor CursorColor = FLinearColor::White;
};
