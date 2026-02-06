// Fill out your copyright notice in the Description page of Project Settings.


#include "Directories/SGDirectoriesModule.h"

#include "Core/SGSetup.h"

#define LOCTEXT_NAMESPACE "StyleGuideDirectoryModule"

void USGDirectoriesModule::MergeModuleSettings_Implementation(USGModule* Module)
{
	if (const USGDirectoriesModule* DirectoriesModule = Cast<USGDirectoriesModule>(Module))
	{
		FString PackagePath = Module->GetPathName();
		PackagePath.LeftInline(PackagePath.Find("/", ESearchCase::IgnoreCase, ESearchDir::FromEnd));
		
		if (DirectoriesModule->Override_DisallowedDirectories)
		{
			DisallowedDirectories = DirectoriesModule->DisallowedDirectories;
		}
		if (AllowedDirectories.Contains(PackagePath) || AllowedDirectories.IsEmpty())
		{
			SGDirectory = PackagePath;
			AllowedDirectories = DirectoriesModule->AllowedDirectories;
			for (FString& AllowedDirectory: AllowedDirectories)
			{
				AllowedDirectory = PackagePath + "/" + AllowedDirectory;
			}
		}
		bDisallowAssetsInCurrentDirectory = DirectoriesModule->bDisallowAssetsInCurrentDirectory;
	}
	
}

EDataValidationResult USGDirectoriesModule::ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
	const FDataValidationContext& Context)
{
	
	if (!AllowedDirectories.IsEmpty())
	{
		bool bIsInAllowedDirectory = InAssetData.PackagePath.ToString() == SGDirectory && !bDisallowAssetsInCurrentDirectory;
		if (!bIsInAllowedDirectory)
		{
			for (const FString& AllowedDirectory: AllowedDirectories)
			{
				if (InAssetData.PackagePath.ToString().StartsWith(AllowedDirectory))
				{
					bIsInAllowedDirectory = true;
					break;
				}
			}
		}
		if (!bIsInAllowedDirectory)
		{
			FString AllowedDirectoriesString;
			for (const FString& AllowedDirectory: AllowedDirectories)
			{
				AllowedDirectoriesString.Append(FString::Printf(TEXT("\n- %s"), *AllowedDirectory));
			}
			FFormatNamedArguments Args;
			Args.Add(TEXT("AllowList"), FText::FromString(FString::Printf(TEXT("%s"), *AllowedDirectoriesString)));
			Args.Add(TEXT("Path"), FText::FromName(InAssetData.PackagePath));

			const FText Message = FText::Format(
				LOCTEXT("NotInAllowedDirectory", "Asset is placed outside of Allowed Directories: {AllowList} \n Please place it in a valid directory"), Args);
			Result = SubmitValidationFailEvent(ValidationVerbosity, InAsset, Message);
		}
	}
	if (!DisallowedDirectories.IsEmpty())
	{
		const FString RelativePath = InAssetData.PackagePath.ToString().RightChop(SGDirectory.Len());
		for (const FString& DisallowedDirectory: DisallowedDirectories)
		{
			if (RelativePath.Contains(DisallowedDirectory))
			{				
				FFormatNamedArguments Args;
				Args.Add(TEXT("Directory"), FText::FromString(DisallowedDirectory));
				Args.Add(TEXT("Path"), FText::FromName(InAssetData.PackagePath));

				const FText Message = FText::Format(
					LOCTEXT("InDisallowedDirectory", "Asset is placed inside of a Disallowed Directory: {Directory}"), Args);
				Result = SubmitValidationFailEvent(ValidationVerbosity, InAsset, Message);
				break;
			
			}
		}
	}
	if (InAssetData.PackagePath.ToString() == SGDirectory && bDisallowAssetsInCurrentDirectory && 
		!InAsset->GetClass()->IsChildOf(USGSetup::StaticClass()))
	{
		FFormatNamedArguments Args;
		Args.Add(TEXT("Directory"), FText::FromString(SGDirectory));
		Args.Add(TEXT("Path"), FText::FromName(InAssetData.PackagePath));

		const FText Message = FText::Format(
			LOCTEXT("InDisallowedCurrentDirectory", "Asset is placed inside of a Disallowed Directory: {Directory}"), Args);
		Result = SubmitValidationFailEvent(ValidationVerbosity, InAsset, Message);
	}
	return Result;
	
}

bool USGDirectoriesModule::CanValidateAsset(const FAssetData& AssetData, UObject* Object,
	FDataValidationContext& Context) const
{
	return ValidationVerbosity != ESGValidationVerbosity::None && Super::CanValidateAsset(AssetData, Object, Context);
}

#undef LOCTEXT_NAMESPACE
