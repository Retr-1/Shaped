#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapedGameModeBase.generated.h"

class ABaseCore;
class AShapedGameStateBase;

UCLASS()
class SHAPED_API AShapedGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AShapedGameModeBase();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void StartPreparationPhase();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void StartCombatPhase();

	UFUNCTION(BlueprintCallable, Category = "Game Flow")
	void EndRun(bool bWasSuccessful);

	UFUNCTION(BlueprintPure, Category = "Game Flow")
	float GetPhaseTimeRemaining() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow", meta = (ClampMin = "1.0"))
	float PreparationPhaseDuration = 90.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow", meta = (ClampMin = "1.0"))
	float CombatPhaseDuration = 60.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	TSubclassOf<ABaseCore> BaseCoreClass;

	UPROPERTY(BlueprintReadOnly, Category = "Game Flow")
	TObjectPtr<ABaseCore> SpawnedBaseCore;

	UPROPERTY()
	TObjectPtr<AShapedGameStateBase> ShapedGameState;

	FTimerHandle PhaseTimerHandle;
	double PhaseEndTimeSeconds = 0.0;

	UFUNCTION()
	void HandleBaseDestroyed();

	UFUNCTION()
	void HandlePlayerDied();

	void SetPhase(EShapedGamePhase NewPhase, float DurationSeconds);
	void SpawnBaseCoreIfNeeded();
	void BindPlayerEvents();
};
