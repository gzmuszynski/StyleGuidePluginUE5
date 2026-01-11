// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGValidator.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/SGModule.h"
#include "Core/SGSetup.h"


EDataValidationResult USGValidator::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData,
	UObject* InAsset, FDataValidationContext& Context)
{
	// TODO: Ensure the assets will be sorted desc by hierarchy.
	
	TArray<USGSetup*> CachedSetups = GetStyleGuideSetupAssets(InAssetData);
	const auto CachedSetup = NewObject<USGSetup>(this);
	TMap<TSubclassOf<USGModule>, USGModule*> CachedModules;
	
	for (USGSetup* Setup : CachedSetups)
	{
		if(Setup)
		{
			for (USGModule* Module :Setup->Settings.Modules)
			{
				if (Module)
				{
					UClass* ModuleClass = Module->GetClass();
					if (!CachedModules.Contains(ModuleClass))
					{
						CachedModules.Add(ModuleClass, CachedSetup->Settings.Modules.Add_GetRef(
							NewObject<USGModule>(CachedSetup, ModuleClass)));
					}
					USGModule* CachedModule = CachedModules[ModuleClass];

					CachedModule->MergeModuleSettings(Module);
				}
			}
		}
	}
	
	for (USGModule* Module : CachedSetup->Settings.Modules)
	{
		if (Module->CanValidateAsset(InAssetData, InAsset, Context))
		{
			Module->ValidateLoadedAsset(InAssetData, InAsset, Context);

			for (FSGValidationMessage Message : Module->GetValidationMessages())
			{
				switch (Message.Severity)
				{
					case EMessageSeverity::Warning:
					{
						AssetWarning(InAsset, Message.Text);
						break;
					}
					case EMessageSeverity::Error:
					{
						AssetFails(InAsset, Message.Text);
						break;
					}
					case EMessageSeverity::Info:
					{
						AssetMessage(InAssetData, Message.Severity, Message.Text);
						break;
					}
					default:{}
				}
			}
		}
	}
	if (GetValidationResult() == EDataValidationResult::NotValidated)
	{
		AssetPasses(InAsset);
	}

	return GetValidationResult();
}

bool USGValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
	FDataValidationContext& InContext) const
{
	return true;
}

TArray<USGSetup*> USGValidator::GetStyleGuideSetupAssets(const FAssetData& InAssetData) const
{
	TArray<USGSetup*> SGSetups;
	
	if(const auto SGDeveloperSettings = GetMutableDefault<USGDeveloperSettings>())
	{
		if(USGSetup* Setup = SGDeveloperSettings->GlobalSetup.Get())
		{
			SGSetups.Add(Setup);
		}
	}

	const IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

	TArray<FAssetData> RedirectAssets;
	AssetRegistry.GetAssetsByClass(USGSetup::StaticClass()->GetClassPathName(), RedirectAssets);
	for(const FAssetData& AssetData : RedirectAssets)
	{
		if(InAssetData.PackagePath == AssetData.PackagePath ||
			InAssetData.PackagePath.ToString().Contains(AssetData.PackagePath.ToString()))
		{
			SGSetups.AddUnique(Cast<USGSetup>(AssetData.GetAsset()));
		}
	}
	return SGSetups;
}
