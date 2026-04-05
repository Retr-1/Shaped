#include "Gameplay/ShapedPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Core/ShapedGameStateBase.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Gameplay/ShapeObject.h"
#include "Kismet/KismetSystemLibrary.h"

AShapedPlayerCharacter::AShapedPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->JumpZVelocity = 450.0f;
	GetCharacterMovement()->AirControl = 0.2f;

	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-10.0f, 0.0f, 64.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;

	FirstPersonHandsMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonHandsMesh"));
	FirstPersonHandsMesh->SetupAttachment(FirstPersonCamera);
	FirstPersonHandsMesh->SetOnlyOwnerSee(true);
	FirstPersonHandsMesh->bCastDynamicShadow = false;
	FirstPersonHandsMesh->CastShadow = false;
	FirstPersonHandsMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -155.0f));
	FirstPersonHandsMesh->SetRelativeRotation(FRotator(2.0f, -15.0f, 5.0f));

	GetMesh()->SetOwnerNoSee(true);
}

void AShapedPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	CurrentHealth = MaxHealth;
	OnPlayerHealthChanged.Broadcast(CurrentHealth, MaxHealth);
}

void AShapedPlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	UpdateHoveredShape();
	UpdateDraggedShape(DeltaSeconds);

	if (bIsDragging)
	{
		DrawDragDebugPoint();
	}
}

void AShapedPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShapedPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShapedPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShapedPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShapedPlayerCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AShapedPlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShapedPlayerCharacter::HandlePrimaryActionPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AShapedPlayerCharacter::HandlePrimaryActionReleased);
	PlayerInputComponent->BindAction(TEXT("IncreaseDragDistance"), IE_Pressed, this, &AShapedPlayerCharacter::IncreaseDragDistance);
	PlayerInputComponent->BindAction(TEXT("DecreaseDragDistance"), IE_Pressed, this, &AShapedPlayerCharacter::DecreaseDragDistance);
}

void AShapedPlayerCharacter::ApplyDamageToPlayer(float DamageAmount)
{
	if (DamageAmount <= 0.0f || CurrentHealth <= 0.0f)
	{
		return;
	}

	CurrentHealth = FMath::Max(0.0f, CurrentHealth - DamageAmount);
	OnPlayerHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		OnPlayerDied.Broadcast();
	}
}

void AShapedPlayerCharacter::FireCurrentAmmo_Implementation()
{
}

void AShapedPlayerCharacter::Interact_Implementation()
{
}

void AShapedPlayerCharacter::StartDragging_Implementation()
{
	if (CurrentHoveredShape)
	{
		CurrentDraggedShape = CurrentHoveredShape;
		CurrentDraggedShape->SetHighlighted(true);
		CurrentHoveredShape = nullptr;
		bIsDragging = true;
	}
}

void AShapedPlayerCharacter::StopDragging_Implementation()
{
	if (CurrentDraggedShape)
	{
		CurrentDraggedShape->SetHighlighted(false);
	}

	bIsDragging = false;
	CurrentDraggedShape = nullptr;
}

void AShapedPlayerCharacter::MoveForward(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AShapedPlayerCharacter::MoveRight(float Value)
{
	if (Controller && !FMath::IsNearlyZero(Value))
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AShapedPlayerCharacter::TurnAtRate(float Value)
{
	AddControllerYawInput(Value);
}

void AShapedPlayerCharacter::LookUpAtRate(float Value)
{
	AddControllerPitchInput(Value);
}

void AShapedPlayerCharacter::HandlePrimaryActionPressed()
{
	switch (GetCurrentGamePhase())
	{
	case EShapedGamePhase::Preparation:
		StartDragging();
		break;
	case EShapedGamePhase::Combat:
		FireCurrentAmmo();
		break;
	default:
		bIsDragging = false;
		break;
	}
}

void AShapedPlayerCharacter::HandlePrimaryActionReleased()
{
	if (GetCurrentGamePhase() == EShapedGamePhase::Preparation)
	{
		StopDragging();
	}
}

EShapedGamePhase AShapedPlayerCharacter::GetCurrentGamePhase() const
{
	if (const AShapedGameStateBase* ShapedGameState = GetWorld() ? GetWorld()->GetGameState<AShapedGameStateBase>() : nullptr)
	{
		return ShapedGameState->GetCurrentPhase();
	}

	return EShapedGamePhase::Preparation;
}

FVector AShapedPlayerCharacter::GetCurrentDragTargetLocation() const
{
	if (!FirstPersonCamera)
	{
		return GetActorLocation();
	}

	return FirstPersonCamera->GetComponentLocation() + (FirstPersonCamera->GetForwardVector() * DragDistance);
}

void AShapedPlayerCharacter::AdjustDragDistance(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	DragDistance = FMath::Clamp(DragDistance + (Value * DragScrollStep), MinDragDistance, GetMaxVirtualDragDistance());
	UKismetSystemLibrary::PrintString(this, FString::Printf(TEXT("DragDistance: %.1f"), DragDistance), true, true, FLinearColor::Yellow, 0.0f);
}

void AShapedPlayerCharacter::IncreaseDragDistance()
{
	AdjustDragDistance(1.0f);
}

void AShapedPlayerCharacter::DecreaseDragDistance()
{
	AdjustDragDistance(-1.0f);
}

float AShapedPlayerCharacter::GetMaxVirtualDragDistance() const
{
	return FMath::Max(MinDragDistance, MaxDragDistance - DragDistanceSafetyMargin);
}

void AShapedPlayerCharacter::UpdateHoveredShape()
{
	if (!GetWorld() || !FirstPersonCamera)
	{
		SetHoveredShape(nullptr);
		return;
	}

	if (bIsDragging || CurrentDraggedShape)
	{
		SetHoveredShape(nullptr);
		return;
	}

	FHitResult HitResult;
	const FVector TraceStart = FirstPersonCamera->GetComponentLocation();
	const FVector TraceEnd = TraceStart + (FirstPersonCamera->GetForwardVector() * HoverTraceDistance);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerHoverTrace), false, this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);

	AShapeObject* NewHoveredShape = bHit ? Cast<AShapeObject>(HitResult.GetActor()) : nullptr;
	if (NewHoveredShape)
	{
		const float DistanceToHoveredShape = FVector::Dist(GetActorLocation(), NewHoveredShape->GetActorLocation());
		if (DistanceToHoveredShape > MaxDragDistance)
		{
			NewHoveredShape = nullptr;
		}
	}

	SetHoveredShape(NewHoveredShape);
}

void AShapedPlayerCharacter::SetHoveredShape(AShapeObject* NewHoveredShape)
{
	if (CurrentHoveredShape == NewHoveredShape)
	{
		return;
	}

	if (CurrentHoveredShape)
	{
		CurrentHoveredShape->SetHighlighted(false);
	}

	CurrentHoveredShape = NewHoveredShape;

	if (CurrentHoveredShape)
	{
		CurrentHoveredShape->SetHighlighted(true);
	}
}

void AShapedPlayerCharacter::UpdateDraggedShape(float DeltaSeconds)
{
	if (!bIsDragging || !CurrentDraggedShape || GetCurrentGamePhase() != EShapedGamePhase::Preparation)
	{
		return;
	}

	UStaticMeshComponent* DraggedMeshComponent = CurrentDraggedShape->GetShapeMeshComponent();
	if (!DraggedMeshComponent || !DraggedMeshComponent->IsSimulatingPhysics())
	{
		return;
	}

	if (!IsDraggedShapeOnScreen())
	{
		StopDragging();
		return;
	}

	const float DistanceToDraggedObject = FVector::Dist(GetActorLocation(), CurrentDraggedShape->GetActorLocation());
	if (DistanceToDraggedObject > MaxDragDistance)
	{
		StopDragging();
		return;
	}

	const FVector TargetLocation = GetCurrentDragTargetLocation();
	const FVector CurrentLocation = DraggedMeshComponent->GetComponentLocation();
	const FVector ToTarget = TargetLocation - CurrentLocation;

	if (ToTarget.IsNearlyZero())
	{
		return;
	}

	const FVector PullDirection = ToTarget.GetSafeNormal();
	const float DistanceScale = FMath::Max(1.0f, ToTarget.Size() / 100.0f);
	const FVector PullForce = PullDirection * DragForce * DistanceScale;
	DraggedMeshComponent->AddForce(PullForce, NAME_None, false);
}

void AShapedPlayerCharacter::DrawDragDebugPoint() const
{
	if (!GetWorld())
	{
		return;
	}

	DrawDebugSphere(
		GetWorld(),
		GetCurrentDragTargetLocation(),
		12.0f,
		16,
		FColor::Cyan,
		false,
		0.0f,
		0,
		1.5f);
}

bool AShapedPlayerCharacter::IsDraggedShapeOnScreen() const
{
	if (!CurrentDraggedShape || !GetWorld())
	{
		return false;
	}

	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
	{
		return false;
	}

	FVector2D ScreenPosition;
	const bool bProjected = PlayerController->ProjectWorldLocationToScreen(CurrentDraggedShape->GetActorLocation(), ScreenPosition, true);
	if (!bProjected)
	{
		return false;
	}

	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	const UStaticMeshComponent* DraggedMeshComponent = CurrentDraggedShape->GetShapeMeshComponent();
	if (!DraggedMeshComponent)
	{
		return ScreenPosition.X >= 0.0f
			&& ScreenPosition.X <= static_cast<float>(ViewportSizeX)
			&& ScreenPosition.Y >= 0.0f
			&& ScreenPosition.Y <= static_cast<float>(ViewportSizeY);
	}

	const FBoxSphereBounds MeshBounds = DraggedMeshComponent->Bounds;
	const FVector BoundsOrigin = MeshBounds.Origin;
	const FVector BoundsExtent = MeshBounds.BoxExtent;

	const TArray<FVector> BoundingPoints =
	{
		BoundsOrigin + FVector( BoundsExtent.X,  BoundsExtent.Y,  BoundsExtent.Z),
		BoundsOrigin + FVector( BoundsExtent.X,  BoundsExtent.Y, -BoundsExtent.Z),
		BoundsOrigin + FVector( BoundsExtent.X, -BoundsExtent.Y,  BoundsExtent.Z),
		BoundsOrigin + FVector( BoundsExtent.X, -BoundsExtent.Y, -BoundsExtent.Z),
		BoundsOrigin + FVector(-BoundsExtent.X,  BoundsExtent.Y,  BoundsExtent.Z),
		BoundsOrigin + FVector(-BoundsExtent.X,  BoundsExtent.Y, -BoundsExtent.Z),
		BoundsOrigin + FVector(-BoundsExtent.X, -BoundsExtent.Y,  BoundsExtent.Z),
		BoundsOrigin + FVector(-BoundsExtent.X, -BoundsExtent.Y, -BoundsExtent.Z)
	};

	for (const FVector& BoundingPoint : BoundingPoints)
	{
		FVector2D BoundingScreenPosition;
		if (!PlayerController->ProjectWorldLocationToScreen(BoundingPoint, BoundingScreenPosition, true))
		{
			continue;
		}

		if (BoundingScreenPosition.X >= 0.0f
			&& BoundingScreenPosition.X <= static_cast<float>(ViewportSizeX)
			&& BoundingScreenPosition.Y >= 0.0f
			&& BoundingScreenPosition.Y <= static_cast<float>(ViewportSizeY))
		{
			return true;
		}
	}

	return false;
}
