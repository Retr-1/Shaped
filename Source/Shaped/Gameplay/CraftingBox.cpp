#include "Gameplay/CraftingBox.h"

#include "Algo/RandomShuffle.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
#include "Core/ShapedGameInstance.h"
#include "Gameplay/ShapeObject.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/CraftingBoxWidget.h"

ACraftingBox::ACraftingBox()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(Root);
	TriggerBox->SetCollisionProfileName(TEXT("Trigger"));
	TriggerBox->SetGenerateOverlapEvents(true);

	ProgressWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("ProgressWidget"));
	ProgressWidgetComponent->SetupAttachment(Root);
	ProgressWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	ProgressWidgetComponent->SetDrawAtDesiredSize(false);
	ProgressWidgetComponent->SetDrawSize(FVector2D(160.0f, 200.0f));
	ProgressWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	ProgressWidgetComponent->SetWidgetClass(UCraftingBoxWidget::StaticClass());
}

void ACraftingBox::BeginPlay()
{
	Super::BeginPlay();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ACraftingBox::HandleTriggerBeginOverlap);
	TriggerBox->OnComponentEndOverlap.AddDynamic(this, &ACraftingBox::HandleTriggerEndOverlap);
	
	if (UCraftingBoxWidget* CraftingWidget = Cast<UCraftingBoxWidget>(ProgressWidgetComponent->GetUserWidgetObject()))
	{
		CraftingWidget->SetCraftingBox(this);
	}
}

void ACraftingBox::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	RemoveInvalidShapes();

	if (!bIsForging)
	{
		if (GetUsableShapeCount() == RequiredShapeCount)
		{
			StartForging();
		}

		return;
	}

	if (GetUsableShapeCount() != RequiredShapeCount)
	{
		StopForging();
		return;
	}

	ForgeElapsedTime += DeltaSeconds;
	if (ForgeElapsedTime >= ForgeDuration)
	{
		FinishForging();
	}
}

float ACraftingBox::GetForgeProgress() const
{
	if (!bIsForging || ForgeDuration <= 0.0f)
	{
		return 0.0f;
	}

	return FMath::Clamp(ForgeElapsedTime / ForgeDuration, 0.0f, 1.0f);
}

int32 ACraftingBox::GetUsableShapeCount() const
{
	int c = 0;
	for (AShapeObject* ShapeObject : ContainedShapes)
	{
		if (IsShapeUsableForForging(ShapeObject))
		{
			c++;
		}
	}
	return  c;
}

void ACraftingBox::HandleTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	AShapeObject* ShapeObject = Cast<AShapeObject>(OtherActor);
	if (!IsValid(ShapeObject) || ContainedShapes.Contains(ShapeObject))
	{
		return;
	}

	ContainedShapes.Add(ShapeObject);
}

void ACraftingBox::HandleTriggerEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	AShapeObject* ShapeObject = Cast<AShapeObject>(OtherActor);
	if (!ShapeObject)
	{
		return;
	}

	ContainedShapes.Remove(ShapeObject);

	if (bIsForging)
	{
		StopForging();
	}
}

void ACraftingBox::StartForging()
{
	bIsForging = true;
	ForgeElapsedTime = 0.0f;
}

void ACraftingBox::StopForging()
{
	bIsForging = false;
	ForgeElapsedTime = 0.0f;
}

void ACraftingBox::FinishForging()
{
	const FName AmmoId = BuildAmmoId();

	if (UShapedGameInstance* GameInstance = GetGameInstance<UShapedGameInstance>())
	{
		GameInstance->PushAmmo(AmmoId);
	}

	const TArray<TObjectPtr<AShapeObject>> ShapesToConsume = ContainedShapes;
	ContainedShapes.Reset();

	for (AShapeObject* ShapeObject : ShapesToConsume)
	{
		if (IsValid(ShapeObject))
		{
			ShapeObject->Destroy();
		}
	}
	
	StopForging();
}

void ACraftingBox::RemoveInvalidShapes()
{
	ContainedShapes.RemoveAll([](const TObjectPtr<AShapeObject>& ShapeObject)
	{
		return !IsValid(ShapeObject);
	});
}

FName ACraftingBox::BuildAmmoId() const
{
	TArray<TCHAR> ColorInitials;
	ColorInitials.Reserve(ContainedShapes.Num());

	for (const AShapeObject* ShapeObject : ContainedShapes)
	{
		ColorInitials.Add(ShapeColorToInitial(ShapeObject->GetShapeColor()));
	}

	Algo::RandomShuffle(ColorInitials);

	FString AmmoIdString;
	AmmoIdString.Reserve(ColorInitials.Num());
	for (const TCHAR ColorInitial : ColorInitials)
	{
		AmmoIdString.AppendChar(ColorInitial);
	}

	return FName(*AmmoIdString);
}

TCHAR ACraftingBox::ShapeColorToInitial(EShapeColor ShapeColor)
{
	switch (ShapeColor)
	{
	case EShapeColor::Red:
		return TEXT('r');
	case EShapeColor::Blue:
		return TEXT('b');
	case EShapeColor::Green:
		return TEXT('g');
	case EShapeColor::Yellow:
		return TEXT('y');
	case EShapeColor::Gray:
		return TEXT('x');
	case EShapeColor::Wildcard:
		return TEXT('w');
	default:
		return TEXT('x');
	}
}

bool ACraftingBox::IsShapeUsableForForging(const AShapeObject* ShapeObject)
{
	return IsValid(ShapeObject) && ShapeObject->GetShapeColor() != EShapeColor::Gray;
}
