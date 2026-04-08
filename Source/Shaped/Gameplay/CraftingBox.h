#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ShapedGameplayTypes.h"
#include "CraftingBox.generated.h"

class AShapeObject;
class UBoxComponent;
class USceneComponent;
class UWidgetComponent;

UCLASS(Blueprintable)
class SHAPED_API ACraftingBox : public AActor
{
	GENERATED_BODY()

public:
	ACraftingBox();

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintPure, Category = "Crafting")
	bool IsForging() const { return bIsForging; }

	UFUNCTION(BlueprintPure, Category = "Crafting")
	float GetForgeProgress() const;

	UFUNCTION(BlueprintPure, Category = "Crafting")
	int32 GetContainedShapeCount() const { return ContainedShapes.Num(); }

	UFUNCTION(BlueprintPure, Category = "Crafting")
	int32 GetUsableShapeCount() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<USceneComponent> Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Crafting")
	TObjectPtr<UWidgetComponent> ProgressWidgetComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting", meta = (ClampMin = "0.1"))
	float ForgeDuration = 3.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Crafting")
	int32 RequiredShapeCount = 3;

	UFUNCTION()
	void HandleTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void HandleTriggerEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

private:
	static bool IsShapeUsableForForging(const AShapeObject* ShapeObject);
	void StartForging();
	void StopForging();
	void FinishForging();
	void RemoveInvalidShapes();
	TArray<TObjectPtr<AShapeObject>> GetUsableShapes() const;
	FName BuildAmmoId() const;
	static TCHAR ShapeColorToInitial(EShapeColor ShapeColor);

	UPROPERTY(Transient)
	TArray<TObjectPtr<AShapeObject>> ContainedShapes;


	bool bIsForging = false;
	float ForgeElapsedTime = 0.0f;
};
