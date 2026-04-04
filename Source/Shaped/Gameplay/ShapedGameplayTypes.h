#pragma once

#include "CoreMinimal.h"
#include "ShapedGameplayTypes.generated.h"

UENUM(BlueprintType)
enum class EShapedGamePhase : uint8
{
	Preparation,
	Combat,
	GameOver
};

UENUM(BlueprintType)
enum class EShapeType : uint8
{
	Box,
	Sphere,
	Pyramid,
	Dodecahedron,
	Wildcard
};

UENUM(BlueprintType)
enum class EShapeColor : uint8
{
	Red,
	Blue,
	Green,
	Yellow,
	Gray,
	Wildcard
};

USTRUCT(BlueprintType)
struct SHAPED_API FAmmoDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	FName AmmoId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	EShapeType ShapeType = EShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	EShapeColor ShapeColor = EShapeColor::Red;
};

USTRUCT(BlueprintType)
struct SHAPED_API FRecipeRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	EShapeType ShapeType = EShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	EShapeColor ShapeColor = EShapeColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta = (ClampMin = "1"))
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct SHAPED_API FEnemyAmmoRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<FName> RequiredAmmoSequence;
};
