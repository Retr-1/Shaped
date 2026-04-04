#include "Core/ShapedGameStateBase.h"

AShapedGameStateBase::AShapedGameStateBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShapedGameStateBase::SetCurrentPhase(EShapedGamePhase NewPhase)
{
	if (CurrentPhase == NewPhase)
	{
		return;
	}

	const EShapedGamePhase PreviousPhase = CurrentPhase;
	CurrentPhase = NewPhase;
	OnGamePhaseChanged.Broadcast(PreviousPhase, CurrentPhase);
}

void AShapedGameStateBase::AdvanceCycle()
{
	++CurrentCycle;
}
