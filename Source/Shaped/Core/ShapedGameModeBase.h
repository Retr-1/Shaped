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
	
	UFUNCTION(BlueprintPure, Category = "Game Flow")
	EShapedGamePhase GetCurrentPhase() const { return CurrentPhase; }

	UFUNCTION(BlueprintNativeEvent, Category = "Game Flow")
	FText GetPhaseLabel();
	
	virtual FText GetPhaseLabel_Implementation();


protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	EShapedGamePhase CurrentPhase = EShapedGamePhase::Preparation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Game Flow")
	FName NextLevelName = NAME_None;
	
	
	
};
