#include "UI/PlayerStatusWidget.h"

#include "Components/Border.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Gameplay/ShapedPlayerCharacter.h"
#include "Styling/SlateColor.h"
#include "Styling/SlateTypes.h"
#include "Blueprint/WidgetTree.h"

void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	BuildWidgetTree();
}

void UPlayerStatusWidget::NativeDestruct()
{
	UnbindFromCurrentPlayer();
	Super::NativeDestruct();
}

void UPlayerStatusWidget::BindToPlayer(AShapedPlayerCharacter* InPlayerCharacter)
{
	if (BoundPlayerCharacter == InPlayerCharacter)
	{
		return;
	}

	UnbindFromCurrentPlayer();
	BoundPlayerCharacter = InPlayerCharacter;

	if (!BoundPlayerCharacter)
	{
		UpdateHealthDisplay(0.0f, 1.0f);
		UpdateStaminaDisplay(0.0f, 1.0f);
		return;
	}

	BoundPlayerCharacter->OnPlayerHealthChanged.AddDynamic(this, &UPlayerStatusWidget::HandleHealthChanged);
	BoundPlayerCharacter->OnPlayerStaminaChanged.AddDynamic(this, &UPlayerStatusWidget::HandleStaminaChanged);

	UpdateHealthDisplay(BoundPlayerCharacter->GetCurrentHealth(), BoundPlayerCharacter->GetMaxHealth());
	UpdateStaminaDisplay(BoundPlayerCharacter->GetCurrentStamina(), BoundPlayerCharacter->GetMaxStamina());
}

void UPlayerStatusWidget::HandleHealthChanged(float NewHealth, float MaxHealth)
{
	UpdateHealthDisplay(NewHealth, MaxHealth);
}

void UPlayerStatusWidget::HandleStaminaChanged(float NewStamina, float MaxStamina)
{
	UpdateStaminaDisplay(NewStamina, MaxStamina);
}

void UPlayerStatusWidget::BuildWidgetTree()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* RootCanvas = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	WidgetTree->RootWidget = RootCanvas;

	UVerticalBox* StatusBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("StatusBox"));
	UCanvasPanelSlot* StatusBoxSlot = RootCanvas->AddChildToCanvas(StatusBox);
	StatusBoxSlot->SetAnchors(FAnchors(0.0f, 1.0f));
	StatusBoxSlot->SetAlignment(FVector2D(0.0f, 1.0f));
	StatusBoxSlot->SetAutoSize(true);
	StatusBoxSlot->SetPosition(FVector2D(32.0f, -32.0f));

	HealthLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("HealthLabel"));
	HealthLabel->SetText(FText::FromString(TEXT("HEALTH")));
	HealthLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	UVerticalBoxSlot* HealthLabelSlot = StatusBox->AddChildToVerticalBox(HealthLabel);
	HealthLabelSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

	HealthBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("HealthBar"));
	HealthBar->SetFillColorAndOpacity(FLinearColor(0.78f, 0.14f, 0.14f, 0.95f));
	HealthBar->SetPercent(1.0f);
	USizeBox* HealthBarContainer = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("HealthBarContainer"));
	HealthBarContainer->SetWidthOverride(220.0f);
	HealthBarContainer->SetHeightOverride(18.0f);
	HealthBarContainer->AddChild(HealthBar);
	UVerticalBoxSlot* HealthBarSlot = StatusBox->AddChildToVerticalBox(HealthBarContainer);
	HealthBarSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 12.0f));
	HealthBarSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));

	StaminaLabel = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("StaminaLabel"));
	StaminaLabel->SetText(FText::FromString(TEXT("STAMINA")));
	StaminaLabel->SetColorAndOpacity(FSlateColor(FLinearColor::White));
	UVerticalBoxSlot* StaminaLabelSlot = StatusBox->AddChildToVerticalBox(StaminaLabel);
	StaminaLabelSlot->SetPadding(FMargin(0.0f, 0.0f, 0.0f, 4.0f));

	StaminaBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("StaminaBar"));
	StaminaBar->SetFillColorAndOpacity(FLinearColor(0.18f, 0.72f, 0.30f, 0.95f));
	StaminaBar->SetPercent(1.0f);
	USizeBox* StaminaBarContainer = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("StaminaBarContainer"));
	StaminaBarContainer->SetWidthOverride(220.0f);
	StaminaBarContainer->SetHeightOverride(18.0f);
	StaminaBarContainer->AddChild(StaminaBar);
	StatusBox->AddChildToVerticalBox(StaminaBarContainer);
}

void UPlayerStatusWidget::UpdateHealthDisplay(float NewHealth, float MaxHealth)
{
	SetBarPercent(HealthBar, NewHealth, MaxHealth);
}

void UPlayerStatusWidget::UpdateStaminaDisplay(float NewStamina, float MaxStamina)
{
	SetBarPercent(StaminaBar, NewStamina, MaxStamina);
}

void UPlayerStatusWidget::SetBarPercent(UProgressBar* ProgressBar, float CurrentValue, float MaxValue) const
{
	if (!ProgressBar)
	{
		return;
	}

	const float Percent = MaxValue > 0.0f ? FMath::Clamp(CurrentValue / MaxValue, 0.0f, 1.0f) : 0.0f;
	ProgressBar->SetPercent(Percent);
}

void UPlayerStatusWidget::UnbindFromCurrentPlayer()
{
	if (!BoundPlayerCharacter)
	{
		return;
	}

	BoundPlayerCharacter->OnPlayerHealthChanged.RemoveDynamic(this, &UPlayerStatusWidget::HandleHealthChanged);
	BoundPlayerCharacter->OnPlayerStaminaChanged.RemoveDynamic(this, &UPlayerStatusWidget::HandleStaminaChanged);
	BoundPlayerCharacter = nullptr;
}
