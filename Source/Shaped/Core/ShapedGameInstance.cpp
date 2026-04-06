#include "Core/ShapedGameInstance.h"

#include "AssetRegistry/AssetData.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Engine/Texture2D.h"
#include "Logging/LogMacros.h"
#include "Modules/ModuleManager.h"

DEFINE_LOG_CATEGORY_STATIC(LogShapedGameInstance, Log, All);

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

TArray<FName> UShapedGameInstance::GetTopAmmo(int32 MaxCount) const
{
	TArray<FName> Result;

	if (MaxCount <= 0 || AmmoStack.IsEmpty())
	{
		return Result;
	}

	const int32 CountToCopy = FMath::Min(MaxCount, AmmoStack.Num());
	Result.Reserve(CountToCopy);

	for (int32 Index = AmmoStack.Num() - 1; Index >= 0 && Result.Num() < CountToCopy; --Index)
	{
		Result.Add(AmmoStack[Index]);
	}

	return Result;
}

void UShapedGameInstance::ClearAmmo()
{
	AmmoStack.Reset();
}

void UShapedGameInstance::ReloadAmmoIconMap()
{
	AmmoIconMap.Reset();
	LoadedAmmoIcons.Reset();

	if (AmmoIconDirectory.Path.IsEmpty())
	{
		UE_LOG(LogShapedGameInstance, Warning, TEXT("AmmoIconDirectory is empty."));
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

		AmmoIconMap.Add(AssetData.AssetName, AssetData.ToSoftObjectPath());
	}

	UE_LOG(LogShapedGameInstance, Log, TEXT("Loaded %d ammo icons from '%s'."), AmmoIconMap.Num(), *AmmoIconDirectory.Path);
}

bool UShapedGameInstance::HasAmmoIcon(FName AmmoId) const
{
	return !AmmoId.IsNone() && AmmoIconMap.Contains(AmmoId);
}

UTexture2D* UShapedGameInstance::GetAmmoIcon(FName AmmoId)
{
	if (AmmoId.IsNone())
	{
		return nullptr;
	}

	if (TObjectPtr<UTexture2D>* CachedTexture = LoadedAmmoIcons.Find(AmmoId))
	{
		return CachedTexture->Get();
	}

	FSoftObjectPath* IconPath = AmmoIconMap.Find(AmmoId);
	if (!IconPath)
	{
		UE_LOG(LogShapedGameInstance, Warning, TEXT("No ammo icon asset registered for '%s' in '%s'."), *AmmoId.ToString(), *AmmoIconDirectory.Path);
		return nullptr;
	}

	UTexture2D* LoadedTexture = Cast<UTexture2D>(IconPath->TryLoad());
	if (!LoadedTexture)
	{
		UE_LOG(LogShapedGameInstance, Warning, TEXT("Failed to load ammo icon asset '%s' for ammo id '%s'."), *IconPath->ToString(), *AmmoId.ToString());
		return nullptr;
	}

	LoadedAmmoIcons.Add(AmmoId, LoadedTexture);
	return LoadedTexture;
}
