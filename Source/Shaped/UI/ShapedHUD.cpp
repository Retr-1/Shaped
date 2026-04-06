#include "UI/ShapedHUD.h"

#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "Gameplay/ShapedPlayerCharacter.h"

void AShapedHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

	const FVector2D ScreenCenter(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	const FVector2D StatusBarSize(StatusBarWidth, StatusBarHeight);
	const FVector2D StaminaBarPosition(StatusBarPadding, Canvas->ClipY - StatusBarPadding - StatusBarHeight);
	const FVector2D HealthBarPosition(StatusBarPadding, StaminaBarPosition.Y - StatusBarSpacing - StatusBarHeight);

	if (const AShapedPlayerCharacter* PlayerCharacter = Cast<AShapedPlayerCharacter>(GetOwningPawn()))
	{
		const float HealthPercent = PlayerCharacter->GetMaxHealth() > 0.0f ? PlayerCharacter->GetCurrentHealth() / PlayerCharacter->GetMaxHealth() : 0.0f;
		const float StaminaPercent = PlayerCharacter->GetMaxStamina() > 0.0f ? PlayerCharacter->GetCurrentStamina() / PlayerCharacter->GetMaxStamina() : 0.0f;

		DrawStatusBar(HealthBarPosition, StatusBarSize, HealthPercent, HealthBarColor, TEXT("HEALTH"));
		DrawStatusBar(StaminaBarPosition, StatusBarSize, StaminaPercent, StaminaBarColor, TEXT("STAMINA"));
	}

	DrawLine(
		ScreenCenter.X - CursorGap - CursorHalfSize,
		ScreenCenter.Y,
		ScreenCenter.X - CursorGap,
		ScreenCenter.Y,
		CursorColor,
		CursorThickness);

	DrawLine(
		ScreenCenter.X + CursorGap,
		ScreenCenter.Y,
		ScreenCenter.X + CursorGap + CursorHalfSize,
		ScreenCenter.Y,
		CursorColor,
		CursorThickness);

	DrawLine(
		ScreenCenter.X,
		ScreenCenter.Y - CursorGap - CursorHalfSize,
		ScreenCenter.X,
		ScreenCenter.Y - CursorGap,
		CursorColor,
		CursorThickness);

	DrawLine(
		ScreenCenter.X,
		ScreenCenter.Y + CursorGap,
		ScreenCenter.X,
		ScreenCenter.Y + CursorGap + CursorHalfSize,
		CursorColor,
		CursorThickness);
}

void AShapedHUD::DrawStatusBar(const FVector2D& Position, const FVector2D& Size, float FillPercent, const FLinearColor& FillColor, const FString& Label)
{
	if (!Canvas)
	{
		return;
	}

	const float ClampedFillPercent = FMath::Clamp(FillPercent, 0.0f, 1.0f);
	const float InnerWidth = FMath::Max(0.0f, Size.X - (StatusBarOutlineThickness * 2.0f));
	const float InnerHeight = FMath::Max(0.0f, Size.Y - (StatusBarOutlineThickness * 2.0f));

	DrawRect(StatusBarOutlineColor, Position.X, Position.Y, Size.X, Size.Y);
	DrawRect(
		StatusBarBackgroundColor,
		Position.X + StatusBarOutlineThickness,
		Position.Y + StatusBarOutlineThickness,
		InnerWidth,
		InnerHeight);

	if (ClampedFillPercent > 0.0f)
	{
		DrawRect(
			FillColor,
			Position.X + StatusBarOutlineThickness,
			Position.Y + StatusBarOutlineThickness,
			InnerWidth * ClampedFillPercent,
			InnerHeight);
	}

	if (GEngine && GEngine->GetSmallFont())
	{
		DrawText(Label, FLinearColor::White, Position.X, Position.Y - 18.0f, GEngine->GetSmallFont());
	}
}
