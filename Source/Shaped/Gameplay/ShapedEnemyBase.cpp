#include "Gameplay/ShapedEnemyBase.h"
#include "UI/EnemyStatusWidget.h"
#include "Gameplay/BaseCore.h"

AShapedEnemyBase::AShapedEnemyBase()
{
	PrimaryActorTick.bCanEverTick = true;
	
	OverheadWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("OverheadWidgetComponent"));
	OverheadWidgetComponent->SetupAttachment(GetRootComponent());
	OverheadWidgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 120.0f));
	OverheadWidgetComponent->SetWidgetSpace(EWidgetSpace::World);
	OverheadWidgetComponent->SetDrawSize(FVector2D(200.f, 50.f));

}

bool AShapedEnemyBase::ApplyAmmoHit(FName AmmoId)
{
	bool WasHit = false;
	if (IsHitWithCorrectAmmo(AmmoId))
	{
		Hits++;
		WasHit = true;
		OnHit.Broadcast(this);
	}
	
	if (Hits >= AmmoRequirement.Num())
	{
		Destroy();
	}
	return WasHit;
}

bool AShapedEnemyBase::IsHitWithCorrectAmmo(FName AmmoId)
{
	TArray<TCHAR> IncomingAmmo = AmmoId.ToString().GetCharArray();
	TArray<TCHAR> Requirement = AmmoRequirement[Hits].ToString().GetCharArray();
	
	const TArray<TCHAR> Colors = {
		TEXT('r'),
		TEXT('g'),
		TEXT('b'),
		TEXT('y'),
	};
	
	const TCHAR wildcard = TEXT('w');
	
	int Matched = 0;
	
	for (const TCHAR &Color : Colors) {
		int InC = 0;
		int ReqC = 0;
		
		for (int i=0; i<3; i++) {
			InC += IncomingAmmo[i] == Color;
			ReqC += Requirement[i] == Color;
		}
		
		Matched += FMath::Min(InC, ReqC);
	}
	
	for (int i=0; i<3; i++) {
		Matched += IncomingAmmo[i] == wildcard;
		Matched += Requirement[i] == wildcard;
	}
	
	return Matched >= 3;
}

int AShapedEnemyBase::GetHits() {
	return Hits;
}

void AShapedEnemyBase::BeginPlay() {
	Super::BeginPlay();
	
	OverheadWidgetComponent->SetWidgetClass(OverheadWidgetClass);
	OverheadWidgetComponent->InitWidget();
	if (UEnemyStatusWidget* StatusWidget = Cast<UEnemyStatusWidget>(OverheadWidgetComponent->GetWidget())) {
		StatusWidget->SetOwningEnemy(this);
	}
	
	OnHit.Broadcast(this);
}

