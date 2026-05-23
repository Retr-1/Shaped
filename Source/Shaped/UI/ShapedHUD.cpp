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
