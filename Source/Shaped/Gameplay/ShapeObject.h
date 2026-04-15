#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gameplay/ShapedGameplayTypes.h"
#include "ShapeObject.generated.h"

class UMaterialInterface;
class UMaterialInstanceDynamic;
class UStaticMesh;
class UStaticMeshComponent;

UCLASS(Blueprintable)
class SHAPED_API AShapeObject : public AActor
{
	GENERATED_BODY()

public:
	AShapeObject();

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Shape")
	void RefreshShapePresentation();

	UFUNCTION(BlueprintCallable, Category = "Shape")
	void SetHighlighted(bool bHighlighted);

	UFUNCTION(BlueprintPure, Category = "Shape")
	UStaticMeshComponent* GetShapeMeshComponent() const { return ShapeMeshComponent; }

	UFUNCTION(BlueprintPure, Category = "Shape")
	EShapeType GetShapeType() const { return ShapeType; }

	UFUNCTION(BlueprintPure, Category = "Shape")
	EShapeColor GetShapeColor() const { return ShapeColor; }

	UFUNCTION(BlueprintPure, Category = "Shape")
	float GetPhysicsWeight() const { return PhysicsWeight; }

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shape")
	TObjectPtr<UStaticMeshComponent> ShapeMeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape", meta=(ExposeOnSpawn="true"))
	EShapeType ShapeType = EShapeType::Box;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape", meta=(ExposeOnSpawn="true"))
	EShapeColor ShapeColor = EShapeColor::Red;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape", meta = (ClampMin = "0.1"))
	float PhysicsWeight = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Meshes")
	TObjectPtr<UStaticMesh> BoxMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Meshes")
	TObjectPtr<UStaticMesh> SphereMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Meshes")
	TObjectPtr<UStaticMesh> PyramidMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Meshes")
	TObjectPtr<UStaticMesh> DodecahedronMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Meshes")
	TObjectPtr<UStaticMesh> TorusMesh;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Meshes")
	TObjectPtr<UStaticMesh> CylinderMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Material")
	TObjectPtr<UMaterialInterface> BaseMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Material")
	FName ColorParameterName = TEXT("BaseColor");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Material")
	FName HighlightParameterName = TEXT("HighlightAmount");

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Shape|Material")
	int32 HighlightStencilValue = 69;

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> DynamicMaterialInstance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Shape|Material")
	bool bIsHighlighted = false;

	UFUNCTION(BlueprintImplementableEvent, Category = "Shape")
	void OnHighlightChanged(bool bHighlighted);

	virtual void BeginPlay() override;

	UStaticMesh* ResolveMeshForType() const;
	FLinearColor ResolveColor() const;
	void ApplyPhysicsProperties();
	void ApplyMaterialProperties();
};
