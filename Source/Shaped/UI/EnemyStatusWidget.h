// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Gameplay/ShapedEnemyBase.h"
#include "EnemyStatusWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHAPED_API UEnemyStatusWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetOwningEnemy(AShapedEnemyBase* Enemy);
	
protected:

	
	UFUNCTION()
	void OnOwnerHit(AShapedEnemyBase* Character);
	
	UPROPERTY(meta=(BindWidget))
	UVerticalBox* AmmoVerticalBox;
};
