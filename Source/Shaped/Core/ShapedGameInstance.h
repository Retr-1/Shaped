#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ShapedGameInstance.generated.h"

class UTexture2D;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAmmoStackChangedSignature);

UCLASS()
class SHAPED_API UShapedGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY(BlueprintAssignable, Category = "Ammo")
	FOnAmmoStackChangedSignature OnAmmoStackChanged;
	
	UFUNCTION(BlueprintCallable, Category="Ammo")
	void BroadcastAmmoStackChanged();
	
	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void MakeTopAmmoWildcard();

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void PushAmmo(FName AmmoId);

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	bool PopAmmo(FName& OutAmmoId);

	UFUNCTION(BlueprintPure, Category = "Ammo")
	bool PeekAmmo(FName& OutAmmoId) const;

	UFUNCTION(BlueprintCallable, Category = "Ammo")
	void ClearAmmo();

	UFUNCTION(BlueprintPure, Category = "Ammo")
	int32 GetAmmoCount() const { return AmmoStack.Num(); }

	UFUNCTION(BlueprintPure, Category = "Ammo")
	const TArray<FName>& GetAmmoStack() const { return AmmoStack; }

	UFUNCTION(BlueprintPure, Category = "Ammo")
	TArray<FName> GetTopAmmo(int32 MaxCount) const;

	UFUNCTION(BlueprintCallable, Category = "Ammo Icons")
	void ReloadAmmoIconMap();

	UFUNCTION(BlueprintPure, Category = "Ammo Icons")
	bool HasAmmoIcon(FName AmmoId) const;

	UFUNCTION(BlueprintCallable, Category = "Ammo Icons")
	UTexture2D* GetAmmoIcon(FName AmmoId);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo")
	TArray<FName> AmmoStack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ammo Icons")
	FDirectoryPath AmmoIconDirectory;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo Icons")
	TMap<FName, FSoftObjectPath> AmmoIconMap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Ammo Icons")
	TMap<FName, TObjectPtr<UTexture2D>> LoadedAmmoIcons;


};
