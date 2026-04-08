#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CraftingBoxWidget.generated.h"

class ACraftingBox;
class UProgressBar;
class UTextBlock;

UCLASS()
class SHAPED_API UCraftingBoxWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	void SetCraftingBox(ACraftingBox* InCraftingBox);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UProgressBar> ForgeProgressBar;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional))
	TObjectPtr<UTextBlock> StatusText;
	
	UPROPERTY()
	TObjectPtr<ACraftingBox> CraftingBox;

private:
	void UpdateFromCraftingBox();
};
