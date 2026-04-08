#include "UI/CraftingBoxWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/WidgetComponent.h"
#include "Gameplay/CraftingBox.h"
#include "Kismet/KismetSystemLibrary.h"

void UCraftingBoxWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateFromCraftingBox();
}

void UCraftingBoxWidget::SetCraftingBox(ACraftingBox* InCraftingBox)
{
	UKismetSystemLibrary::PrintString(this, "Set");
	CraftingBox = InCraftingBox;
}

void UCraftingBoxWidget::UpdateFromCraftingBox()
{
	if (!CraftingBox)
	{
		UKismetSystemLibrary::PrintString(this, "NOT WOKING");
		return;
	}
	
	ForgeProgressBar->SetPercent(CraftingBox->GetForgeProgress());
	
	if (CraftingBox->IsForging())
	{
		StatusText->SetText(FText::FromString(TEXT("Forging")));
	}
	else
	{
		StatusText->SetText(FText::FromString(FString::Printf(TEXT("%d / 3"), CraftingBox->GetContainedShapeCount())));
	}
	
}
