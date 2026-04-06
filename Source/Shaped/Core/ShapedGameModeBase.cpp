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

void AShapedGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	BindPlayerEvents();
	StartPhase();
}

void AShapedGameModeBase::EndRun()
{
	GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
	PhaseEndTimeSeconds = 0.0;
}

float AShapedGameModeBase::GetPhaseTimeRemaining() const
{
	if (!GetWorld())
	{
		return 0.0f;
	}

	return FMath::Max(0.0, PhaseEndTimeSeconds - GetWorld()->GetTimeSeconds());
}

FText AShapedGameModeBase::GetPhaseTimerLabel() const
{
	return PhaseTimerLabel;
}

void AShapedGameModeBase::HandlePlayerDied()
{
	EndRun();
}

void AShapedGameModeBase::StartPhase()
{
	if (!GetWorld())
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
	PhaseEndTimeSeconds = GetWorld()->GetTimeSeconds() + PhaseDuration;

	if (PhaseDuration > 0.0f)
	{
		GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &AShapedGameModeBase::HandlePhaseTimerExpired, PhaseDuration, false);
	}
}

void AShapedGameModeBase::HandlePhaseTimerExpired()
{
	if (NextLevelName != NAME_None)
	{
		UGameplayStatics::OpenLevel(this, NextLevelName);
	}
}

void AShapedGameModeBase::BindPlayerEvents()
{
	if (!GetWorld())
	{
		return;
	}

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (AShapedPlayerCharacter* PlayerCharacter = Cast<AShapedPlayerCharacter>(PlayerController->GetPawn()))
		{
			PlayerCharacter->OnPlayerDied.AddDynamic(this, &AShapedGameModeBase::HandlePlayerDied);
		}
	}
}
