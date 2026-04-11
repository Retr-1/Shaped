#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "ShapedEnemyBase.generated.h"


class UEnemyStatusWidget;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDefeatedSignature, AShapedEnemyBase*, Enemy);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHitSignature, AShapedEnemyBase*, Enemy);

UCLASS(Blueprintable)
class SHAPED_API AShapedEnemyBase : public ACharacter
{
	GENERATED_BODY()

public:
	AShapedEnemyBase();
	bool ApplyAmmoHit(FName AmmoId);
	bool IsHitWithCorrectAmmo(FName AmmoId);

	UPROPERTY(BlueprintAssignable, Category = "Enemy")
	FOnEnemyDefeatedSignature OnEnemyDefeated;
	
	UPROPERTY(BlueprintAssignable, Category= "Enemy")
	FOnHitSignature OnHit;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> AmmoRequirement;
	
	int GetHits();
protected:
	int Hits = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="UI")
	UWidgetComponent* OverheadWidgetComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UEnemyStatusWidget> OverheadWidgetClass;
	
	virtual void BeginPlay() override;
	
};
