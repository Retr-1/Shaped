#include "UI/ShapedHUD.h"

#include "Engine/Canvas.h"

void AShapedHUD::DrawHUD()
{
	Super::DrawHUD();

	if (!Canvas)
	{
		return;
	}

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
