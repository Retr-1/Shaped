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
struct SHAPED_API FShapeIngredientKey
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	EShapeType ShapeType = EShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	EShapeColor ShapeColor = EShapeColor::Red;

	bool operator==(const FShapeIngredientKey& Other) const
	{
		return ShapeType == Other.ShapeType && ShapeColor == Other.ShapeColor;
	}
};

USTRUCT(BlueprintType)
struct SHAPED_API FRecipeIngredient
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	FShapeIngredientKey IngredientKey;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting", meta = (ClampMin = "1"))
	int32 Count = 1;
};

USTRUCT(BlueprintType)
struct SHAPED_API FAmmoDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	FName AmmoId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	TArray<FRecipeIngredient> Composition;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	bool bIsWildcardAmmo = false;
};

USTRUCT(BlueprintType)
struct SHAPED_API FRecipeDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	FName RecipeId = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	TArray<FRecipeIngredient> Ingredients;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Crafting")
	FName OutputAmmoId = NAME_None;
};

USTRUCT(BlueprintType)
struct SHAPED_API FEnemyAmmoRequirement
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	TArray<FName> RequiredAmmoSequence;
};

FORCEINLINE uint32 GetTypeHash(const FShapeIngredientKey& Key)
{
	return HashCombine(GetTypeHash(static_cast<uint8>(Key.ShapeType)), GetTypeHash(static_cast<uint8>(Key.ShapeColor)));
}
