#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapedGameModeBase.generated.h"

UCLASS()
class SHAPED_API AShapedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShapedGameModeBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void EndRun();

	UFUNCTION(BlueprintPure, Category = "Game Flow")
	float GetPhaseTimeRemaining() const;

	UFUNCTION(BlueprintPure, Category = "Game Flow")
	EShapedGamePhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintPure, Category = "Game Flow")
	FText GetPhaseTimerLabel() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow", meta = (ClampMin = "1.0"))
	float PhaseDuration = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	EShapedGamePhase CurrentPhase = EShapedGamePhase::Preparation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FText PhaseTimerLabel = FText::FromString(TEXT("Next Phase in: "));

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FName NextLevelName = NAME_None;

	FTimerHandle PhaseTimerHandle;
	double PhaseEndTimeSeconds = 0.0;

	UFUNCTION()
	void HandlePlayerDied();

	virtual void StartPhase();
	virtual void HandlePhaseTimerExpired();
	void BindPlayerEvents();
};
