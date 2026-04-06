#pragma once

#include "CoreMinimal.h"
#include "Core/ShapedGameModeBase.h"
#include "ShapedCombatGameMode.generated.h"

class ABaseCore;

UCLASS()
class SHAPED_API AShapedCombatGameMode : public AShapedGameModeBase
{
	GENERATED_BODY()

public:
	AShapedCombatGameMode();

	virtual void BeginPlay() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Game Flow")
	TObjectPtr<ABaseCore> BaseCore;

	UFUNCTION()
	void HandleBaseDestroyed();

	void BindPlacedBaseCore();
};
