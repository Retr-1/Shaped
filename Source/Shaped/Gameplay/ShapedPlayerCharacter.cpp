#include "Gameplay/ShapedPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Core/ShapedGameStateBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/ShapeObject.h"

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
}

void AShapedPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &AShapedPlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &AShapedPlayerCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("Turn"), this, &AShapedPlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &AShapedPlayerCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis(TEXT("AdjustDragDistance"), this, &AShapedPlayerCharacter::AdjustDragDistance);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &AShapedPlayerCharacter::Interact);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Pressed, this, &AShapedPlayerCharacter::HandlePrimaryActionPressed);
	PlayerInputComponent->BindAction(TEXT("Fire"), IE_Released, this, &AShapedPlayerCharacter::HandlePrimaryActionReleased);
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
		bIsDragging = true;
	}
}

void AShapedPlayerCharacter::StopDragging_Implementation()
{
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

void AShapedPlayerCharacter::AdjustDragDistance(float Value)
{
	if (FMath::IsNearlyZero(Value))
	{
		return;
	}

	DragDistance = FMath::Clamp(DragDistance + (Value * DragScrollStep), MinDragDistance, MaxDragDistance);
}

void AShapedPlayerCharacter::UpdateHoveredShape()
{
	if (!GetWorld() || !FirstPersonCamera)
	{
		SetHoveredShape(nullptr);
		return;
	}

	FHitResult HitResult;
	const FVector TraceStart = FirstPersonCamera->GetComponentLocation();
	const FVector TraceEnd = TraceStart + (FirstPersonCamera->GetForwardVector() * HoverTraceDistance);
	FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(PlayerHoverTrace), false, this);

	if (CurrentDraggedShape)
	{
		QueryParams.AddIgnoredActor(CurrentDraggedShape);
	}

	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams);
	SetHoveredShape(bHit ? Cast<AShapeObject>(HitResult.GetActor()) : nullptr);
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
