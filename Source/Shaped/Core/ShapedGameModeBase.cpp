#include "Core/ShapedGameModeBase.h"

#include "Core/ShapedGameStateBase.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/BaseCore.h"
#include "Gameplay/ShapedPlayerCharacter.h"
#include "UI/ShapedHUD.h"

AShapedGameModeBase::AShapedGameModeBase()
{
	GameStateClass = AShapedGameStateBase::StaticClass();
	DefaultPawnClass = AShapedPlayerCharacter::StaticClass();
	BaseCoreClass = ABaseCore::StaticClass();
	HUDClass = AShapedHUD::StaticClass();
}

void AShapedGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	ShapedGameState = GetGameState<AShapedGameStateBase>();
	SpawnBaseCoreIfNeeded();
	BindPlayerEvents();
	StartPreparationPhase();
}

void AShapedGameModeBase::StartPreparationPhase()
{
	SetPhase(EShapedGamePhase::Preparation, PreparationPhaseDuration);
}

void AShapedGameModeBase::StartCombatPhase()
{
	SetPhase(EShapedGamePhase::Combat, CombatPhaseDuration);
}

void AShapedGameModeBase::EndRun(bool bWasSuccessful)
{
	GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
	PhaseEndTimeSeconds = 0.0;

	if (ShapedGameState)
	{
		ShapedGameState->SetCurrentPhase(EShapedGamePhase::GameOver);
	}

	if (!bWasSuccessful)
	{
		for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
		{
			if (APlayerController* PlayerController = Iterator->Get())
			{
				PlayerController->GameHasEnded(nullptr, false);
			}
		}
	}
}

float AShapedGameModeBase::GetPhaseTimeRemaining() const
{
	if (!GetWorld())
	{
		return 0.0f;
	}

	return FMath::Max(0.0, PhaseEndTimeSeconds - GetWorld()->GetTimeSeconds());
}

void AShapedGameModeBase::HandleBaseDestroyed()
{
	EndRun(false);
}

void AShapedGameModeBase::HandlePlayerDied()
{
	EndRun(false);
}

void AShapedGameModeBase::SetPhase(EShapedGamePhase NewPhase, float DurationSeconds)
{
	if (!GetWorld() || !ShapedGameState)
	{
		return;
	}

	GetWorldTimerManager().ClearTimer(PhaseTimerHandle);
	ShapedGameState->SetCurrentPhase(NewPhase);
	PhaseEndTimeSeconds = GetWorld()->GetTimeSeconds() + DurationSeconds;

	if (NewPhase == EShapedGamePhase::Preparation)
	{
		GetWorldTimerManager().SetTimer(PhaseTimerHandle, this, &AShapedGameModeBase::StartCombatPhase, DurationSeconds, false);
		return;
	}

	if (NewPhase == EShapedGamePhase::Combat)
	{
		GetWorldTimerManager().SetTimer(PhaseTimerHandle, [this]()
		{
			if (ShapedGameState)
			{
				ShapedGameState->AdvanceCycle();
			}

			StartPreparationPhase();
		}, DurationSeconds, false);
	}
}

void AShapedGameModeBase::SpawnBaseCoreIfNeeded()
{
	if (!GetWorld() || SpawnedBaseCore || !BaseCoreClass)
	{
		return;
	}
	
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	SpawnedBaseCore = GetWorld()->SpawnActor<ABaseCore>(BaseCoreClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParameters);

	if (SpawnedBaseCore)
	{
		SpawnedBaseCore->OnBaseDestroyed.AddDynamic(this, &AShapedGameModeBase::HandleBaseDestroyed);
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
