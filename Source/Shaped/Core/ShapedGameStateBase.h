#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapedGameStateBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnGamePhaseChangedSignature, EShapedGamePhase, PreviousPhase, EShapedGamePhase, NewPhase);

UCLASS()
class SHAPED_API AShapedGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	AShapedGameStateBase();

	UPROPERTY(BlueprintAssignable, Category = "Game State")
	FOnGamePhaseChangedSignature OnGamePhaseChanged;

	UFUNCTION(BlueprintPure, Category = "Game State")
	EShapedGamePhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintPure, Category = "Game State")
	int32 GetCurrentCycle() const { return CurrentCycle; }

	void SetCurrentPhase(EShapedGamePhase NewPhase);
	void AdvanceCycle();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	EShapedGamePhase CurrentPhase = EShapedGamePhase::Preparation;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game State")
	int32 CurrentCycle = 1;
};
