// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyStatusWidget.h"

#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Core/ShapedGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"


void UEnemyStatusWidget::SetOwningEnemy(AShapedEnemyBase* Character) {
	Character->OnHit.AddDynamic(this, &UEnemyStatusWidget::OnOwnerHit);
	UKismetSystemLibrary::PrintString(this, "GOT PAP");
}

void UEnemyStatusWidget::OnOwnerHit(AShapedEnemyBase* Character) {
	UShapedGameInstance *GI = GetGameInstance<UShapedGameInstance>();
	AmmoVerticalBox->ClearChildren();
	
	for (int i=Character->GetHits(); i<Character->AmmoRequirement.Num(); i++) {
		FName ammoId = Character->AmmoRequirement[i];
		UTexture2D *ammoIcon = GI->GetAmmoIcon(ammoId);
		UImage* Image = NewObject<UImage>(this);
		USizeBox* SizeBox = NewObject<USizeBox>(this);
		const float scale = 0.5;
		SizeBox->SetWidthOverride(254.f*scale);
		SizeBox->SetHeightOverride(45.f*scale);
		
		Image->SetBrushFromTexture(ammoIcon, true);
		SizeBox->AddChild(Image);
		UVerticalBoxSlot* WSlot = AmmoVerticalBox->AddChildToVerticalBox(SizeBox);
		WSlot->SetHorizontalAlignment(HAlign_Center);
		WSlot->SetVerticalAlignment(VAlign_Center);
	}
 }
