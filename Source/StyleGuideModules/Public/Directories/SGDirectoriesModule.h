// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SGModule.h"
#include "Core/SGValidationEnums.h"
#include "SGDirectoriesModule.generated.h"

/**
 * 
 */
UCLASS(DisplayName="Directories")
class STYLEGUIDEMODULES_API USGDirectoriesModule : public USGModule
{
	GENERATED_BODY()
public:
	virtual void MergeModuleSettings_Implementation(USGModule* Module) override;
	
	virtual EDataValidationResult ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
		const FDataValidationContext& Context) override;
	
	virtual bool CanValidateAsset(const FAssetData& AssetData, UObject* Object,
		FDataValidationContext& Context) const override;

protected:
	
	UPROPERTY(Transient, BlueprintReadWrite, Category="Directories")
	FString SGDirectory;
	
	// Severity of the Directory checks.
	UPROPERTY(EditDefaultsOnly, Category="Directories")
	ESGValidationVerbosity ValidationVerbosity = ESGValidationVerbosity::Warning;
	
	// List of directory names that are allowed in the directory of this Style Guide Assets.
	// e.g. "Textures" will allow assets to reside inside "Textures/*" folder, but will produce 
	// a validation exception if an asset was saved to "StaticMesh/*" folder.
	// This list is additive: Leaving this empty will not prevent checking against rules set higher up the path chain.
	UPROPERTY(EditDefaultsOnly, Category="Directories")
	TArray<FString> AllowedDirectories;
	
	// Whether to allow Assets in the same directory as this Style Guide asset.
	// Useful for root level Style Guide, to prevent root-level garbage.
	UPROPERTY(EditDefaultsOnly, Category="Directories")
	bool bDisallowAssetsInCurrentDirectory = false;
	
	UPROPERTY(EditDefaultsOnly, Category="Directories", meta=(InlineEditConditionToggle))
	bool Override_DisallowedDirectories;
	
	// List of directory names, that are disallowed, recursively.
	// e.g. "Textures" will disallow creating a "Textures" directory as well as "AssetName/Textures
	// This list is overriding: Leaving this empty will disable this check for current directory and recursive directories,
	// and will prevent checking against rules set higher up the path chain.
	// This setting is useful for preventing creation of "redundant" folders separating assets types,
	// like it's commonly done on marketplace/fab content packs with directories like
	// "Textures", "SkeletalMeshes", "Animation", etc.
	UPROPERTY(EditDefaultsOnly, Category="Directories", meta=(EditCondition="Override_DisallowedDirectories"))
	TArray<FString> DisallowedDirectories;
};
