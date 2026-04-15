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
	Torus,
	Cylinder,
};

UENUM(BlueprintType)
enum class EShapeColor : uint8
{
	Red = 0,
	Blue = 1,
	Green = 2,
	Yellow = 3,
	Gray = 4,
	Wildcard = 5
};




