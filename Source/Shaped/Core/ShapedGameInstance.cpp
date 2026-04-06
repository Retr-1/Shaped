#include "Core/ShapedGameInstance.h"

#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture2D.h"
#include "Modules/ModuleManager.h"

void UShapedGameInstance::Init()
{
	Super::Init();

	PushAmmo(FName(TEXT("bbr")));
	PushAmmo(FName(TEXT("rgb")));
	PushAmmo(FName(TEXT("yyy")));
	PushAmmo(FName(TEXT("gbw")));

	ReloadAmmoIconMap();
}

void UShapedGameInstance::PushAmmo(FName AmmoId)
{
	if (AmmoId.IsNone())
	{
		return;
	}

	AmmoStack.Add(AmmoId);
}

bool UShapedGameInstance::PopAmmo(FName& OutAmmoId)
{
	if (AmmoStack.IsEmpty())
	{
		OutAmmoId = NAME_None;
		return false;
	}

	OutAmmoId = AmmoStack.Pop(EAllowShrinking::No);
	return true;
}

bool UShapedGameInstance::PeekAmmo(FName& OutAmmoId) const
{
	if (AmmoStack.IsEmpty())
	{
		OutAmmoId = NAME_None;
		return false;
	}

	OutAmmoId = AmmoStack.Last();
	return true;
}

void UShapedGameInstance::ClearAmmo()
{
	AmmoStack.Reset();
}

void UShapedGameInstance::ReloadAmmoIconMap()
{
	AmmoIconMap.Reset();

	if (AmmoIconDirectory.Path.IsEmpty())
	{
		return;
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry"));

	TArray<FAssetData> AssetDataList;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*AmmoIconDirectory.Path), AssetDataList, true, false);

	for (const FAssetData& AssetData : AssetDataList)
	{
		if (!AssetData.IsValid() || !AssetData.AssetClassPath.GetAssetName().IsEqual(TEXT("Texture2D")))
		{
			continue;
		}

		const FName AmmoId = AssetData.AssetName;
		AmmoIconMap.Add(AmmoId, TSoftObjectPtr<UTexture2D>(AssetData.ToSoftObjectPath()));
	}
}

bool UShapedGameInstance::HasAmmoIcon(FName AmmoId) const
{
	return AmmoIconMap.Contains(AmmoId);
}

UTexture2D* UShapedGameInstance::GetAmmoIcon(FName AmmoId)
{
	if (AmmoId.IsNone())
	{
		return nullptr;
	}

	TSoftObjectPtr<UTexture2D>* IconPtr = AmmoIconMap.Find(AmmoId);
	if (!IconPtr)
	{
		return nullptr;
	}

	return IconPtr->LoadSynchronous();
}
