#include "Core/ShapedCombatGameMode.h"

#include "EngineUtils.h"
#include "Gameplay/BaseCore.h"

AShapedCombatGameMode::AShapedCombatGameMode()
{
	CurrentPhase = EShapedGamePhase::Combat;
	PhaseDuration = 60.0f;
	PhaseTimerLabel = FText::FromString(TEXT("NEXT PHASE"));
}

void AShapedCombatGameMode::BeginPlay()
{
	Super::BeginPlay();
	BindPlacedBaseCore();
}

void AShapedCombatGameMode::HandleBaseDestroyed()
{
	EndRun();
}

void AShapedCombatGameMode::BindPlacedBaseCore()
{
	if (!GetWorld() || BaseCore)
	{
		return;
	}

	for (TActorIterator<ABaseCore> It(GetWorld()); It; ++It)
	{
		BaseCore = *It;
		break;
	}

	if (BaseCore)
	{
		BaseCore->OnBaseDestroyed.AddDynamic(this, &AShapedCombatGameMode::HandleBaseDestroyed);
	}
}
