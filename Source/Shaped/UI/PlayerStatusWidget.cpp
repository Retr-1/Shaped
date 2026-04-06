#include "UI/PlayerStatusWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Core/ShapedGameModeBase.h"
#include "Gameplay/ShapedPlayerCharacter.h"

void UPlayerStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();
	UpdatePhaseDisplay();
}

void UPlayerStatusWidget::NativeDestruct()
{
	UnbindFromCurrentPlayer();
	Super::NativeDestruct();
}

void UPlayerStatusWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdatePhaseDisplay();
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

void UPlayerStatusWidget::UpdateHealthDisplay(float NewHealth, float MaxHealth)
{
	if (HealthLabel)
	{
		HealthLabel->SetText(FText::FromString(TEXT("HEALTH")));
	}

	SetBarPercent(HealthBar, NewHealth, MaxHealth);
}

void UPlayerStatusWidget::UpdateStaminaDisplay(float NewStamina, float MaxStamina)
{
	if (StaminaLabel)
	{
		StaminaLabel->SetText(FText::FromString(TEXT("STAMINA")));
	}

	SetBarPercent(StaminaBar, NewStamina, MaxStamina);
}

void UPlayerStatusWidget::UpdatePhaseDisplay()
{
	BoundGameMode = GetWorld() ? Cast<AShapedGameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;

	if (!BoundGameMode)
	{
		if (PhaseLabel)
		{
			PhaseLabel->SetText(FText::GetEmpty());
		}

		if (PhaseTimerText)
		{
			PhaseTimerText->SetText(FText::GetEmpty());
		}

		return;
	}

	if (PhaseLabel)
	{
		PhaseLabel->SetText(BoundGameMode->GetPhaseTimerLabel());
	}

	if (PhaseTimerText)
	{
		const int32 RemainingSeconds = FMath::Max(0, FMath::CeilToInt(BoundGameMode->GetPhaseTimeRemaining()));
		const int32 MinutesPart = RemainingSeconds / 60;
		const int32 SecondsPart = RemainingSeconds % 60;
		PhaseTimerText->SetText(FText::FromString(FString::Printf(TEXT("%02d:%02d"), MinutesPart, SecondsPart)));
	}
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
