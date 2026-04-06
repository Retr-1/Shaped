#include "UI/ShapedHUD.h"

#include "Engine/Canvas.h"
#include "Gameplay/ShapedPlayerCharacter.h"
#include "UI/PlayerStatusWidget.h"

void AShapedHUD::BeginPlay()
{
	Super::BeginPlay();
	InitializeStatusWidget();
	RefreshStatusWidgetBinding();
}

void AShapedHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

	RefreshStatusWidgetBinding();

	const FVector2D ScreenCenter(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

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

void AShapedHUD::InitializeStatusWidget()
{
	if (PlayerStatusWidget)
	{
		return;
	}

	if (!PlayerStatusWidgetClass)
	{
		PlayerStatusWidgetClass = UPlayerStatusWidget::StaticClass();
	}

	if (APlayerController* PlayerController = GetOwningPlayerController())
	{
		PlayerStatusWidget = CreateWidget<UPlayerStatusWidget>(PlayerController, PlayerStatusWidgetClass);
		if (PlayerStatusWidget)
		{
			PlayerStatusWidget->AddToViewport();
		}
	}
}

void AShapedHUD::RefreshStatusWidgetBinding()
{
	if (!PlayerStatusWidget)
	{
		return;
	}

	AShapedPlayerCharacter* CurrentPlayerCharacter = Cast<AShapedPlayerCharacter>(GetOwningPawn());
	if (LastBoundPlayerCharacter == CurrentPlayerCharacter)
	{
		return;
	}

	LastBoundPlayerCharacter = CurrentPlayerCharacter;
	PlayerStatusWidget->BindToPlayer(CurrentPlayerCharacter);
}
