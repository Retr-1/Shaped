#include "Core/ShapedGameModeBase.h"

#include "Engine/World.h"
#include "Gameplay/ShapedPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ShapedHUD.h"

AShapedGameModeBase::AShapedGameModeBase()
{
	DefaultPawnClass = AShapedPlayerCharacter::StaticClass();
	HUDClass = AShapedHUD::StaticClass();
}

FText AShapedGameModeBase::GetPhaseLabel_Implementation()
{
	return FText::FromString("BottomText");
}








