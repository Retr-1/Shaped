#include "Gameplay/ShapeObject.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Materials/MaterialInterface.h"

AShapeObject::AShapeObject()
{
	PrimaryActorTick.bCanEverTick = false;

	ShapeMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShapeMesh"));
	SetRootComponent(ShapeMeshComponent);
	ShapeMeshComponent->SetSimulatePhysics(true);
	ShapeMeshComponent->SetCollisionProfileName(TEXT("PhysicsActor"));
	ShapeMeshComponent->SetRenderCustomDepth(false);
}

void AShapeObject::BeginPlay()
{
	Super::BeginPlay();
	RefreshShapePresentation();
}

void AShapeObject::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	RefreshShapePresentation();
}

void AShapeObject::RefreshShapePresentation()
{
	if (!ShapeMeshComponent)
	{
		return;
	}

	if (UStaticMesh* ResolvedMesh = ResolveMeshForType())
	{
		ShapeMeshComponent->SetStaticMesh(ResolvedMesh);
	}

	ApplyPhysicsProperties();
	ApplyMaterialProperties();
	SetHighlighted(bIsHighlighted);
}

void AShapeObject::SetHighlighted(bool bHighlighted)
{
	bIsHighlighted = bHighlighted;

	if (!ShapeMeshComponent)
	{
		return;
	}

	ShapeMeshComponent->SetRenderCustomDepth(bIsHighlighted);
	ShapeMeshComponent->SetCustomDepthStencilValue(HighlightStencilValue);

	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetScalarParameterValue(HighlightParameterName, bIsHighlighted ? 1.0f : 0.0f);
	}

	OnHighlightChanged(bIsHighlighted);
}

UStaticMesh* AShapeObject::ResolveMeshForType() const
{
	switch (ShapeType)
	{
	case EShapeType::Box:
		return BoxMesh;
	case EShapeType::Sphere:
		return SphereMesh;
	case EShapeType::Pyramid:
		return PyramidMesh;
	case EShapeType::Dodecahedron:
		return DodecahedronMesh;
	case EShapeType::Wildcard:
	default:
		return SphereMesh ? SphereMesh : BoxMesh;
	}
}

FLinearColor AShapeObject::ResolveColor() const
{
	switch (ShapeColor)
	{
	case EShapeColor::Red:
		return FLinearColor(1.0f, 0.15f, 0.15f);
	case EShapeColor::Blue:
		return FLinearColor(0.15f, 0.35f, 1.0f);
	case EShapeColor::Green:
		return FLinearColor(0.1f, 0.85f, 0.2f);
	case EShapeColor::Yellow:
		return FLinearColor(1.0f, 0.85f, 0.1f);
	case EShapeColor::Gray:
		return FLinearColor(0.5f, 0.5f, 0.5f);
	case EShapeColor::Wildcard:
	default:
		return FLinearColor(0.95f, 0.3f, 1.0f);
	}
}

void AShapeObject::ApplyPhysicsProperties()
{
	if (!ShapeMeshComponent)
	{
		return;
	}

	ShapeMeshComponent->SetSimulatePhysics(true);
	ShapeMeshComponent->SetMassOverrideInKg(NAME_None, PhysicsWeight, true);
}

void AShapeObject::ApplyMaterialProperties()
{
	if (!ShapeMeshComponent)
	{
		return;
	}

	if (BaseMaterial)
	{
		DynamicMaterialInstance = UMaterialInstanceDynamic::Create(BaseMaterial, this);
		ShapeMeshComponent->SetMaterial(0, DynamicMaterialInstance);
	}
	else
	{
		DynamicMaterialInstance = Cast<UMaterialInstanceDynamic>(ShapeMeshComponent->GetMaterial(0));
	}

	if (DynamicMaterialInstance)
	{
		DynamicMaterialInstance->SetVectorParameterValue(ColorParameterName, ResolveColor());
		DynamicMaterialInstance->SetScalarParameterValue(HighlightParameterName, bIsHighlighted ? 1.0f : 0.0f);
	}
}
