#include "Core/ShapedPreparationGameMode.h"

AShapedPreparationGameMode::AShapedPreparationGameMode()
{
	CurrentPhase = EShapedGamePhase::Preparation;
	PhaseDuration = 90.0f;
	PhaseTimerLabel = FText::FromString(TEXT("NEXT PHASE"));
}
