// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGModule.h"

#include "AssetNaming/SGAssetNamingModule.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Core/SGSetup.h"

void USGModule::MergeModuleSettings_Implementation(USGModule* Module)
{
	bDisableModuleChecks = Module->bDisableModuleChecks;
}

TArray<FSGValidationMessage> USGModule::GetValidationMessages()
{
	return ValidationMessages;
}

bool USGModule::CanValidateAsset(const FAssetData& InAssetData, UObject* Object, FDataValidationContext& Context) const
{
	if (bDisableModuleChecks)
	{
		return false;
	}
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	
	TArray<FAssetData> RedirectAssets;
	AssetRegistry.GetAssetsByClass(USGSetup::StaticClass()->GetClassPathName(), RedirectAssets);
	for(const FAssetData& AssetData : RedirectAssets)
	{
		if(InAssetData.PackagePath == AssetData.PackagePath ||
			InAssetData.PackagePath.ToString().Contains(AssetData.PackagePath.ToString()))
		{
			return true;
		}
	}
	return false;
}

EDataValidationResult USGModule::ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
                                                     const FDataValidationContext& Context)
{
	return EDataValidationResult::NotValidated;
}

bool USGModule::IsCheckEnabled(FName CheckPropertyName) const
{
	// TODO: Very important function. Otherwise all of the checks will be performed regardless of the setting.
	return true;
}

void USGModule::SubmitMessage(const UObject* Asset, FText Message)
{
	ValidationMessages.Add({EMessageSeverity::Info, Message});
}

EDataValidationResult USGModule::SubmitValidationFailEvent(ESGValidationVerbosity Verbosity, const UObject* Asset,
                                                           FText Message)
{
	switch (Verbosity)
	{
	case ESGValidationVerbosity::None:
		{
			break;
		}
	case ESGValidationVerbosity::Hint:
		{
			ValidationMessages.Add({EMessageSeverity::Info, Message});
			break;
		}
	case ESGValidationVerbosity::Warning:
		{
			ValidationMessages.Add({EMessageSeverity::Warning, Message});
			break;
		}
	case ESGValidationVerbosity::Error:
		{
			Result = EDataValidationResult::Invalid;
			ValidationMessages.Add({EMessageSeverity::Error, Message});
			return Result;
		}
	}
	if (Result == EDataValidationResult::NotValidated)
	{
		Result = EDataValidationResult::Valid;
	}
	return Result;
}

void MergeModuleProperty(FProperty* Property, void* DstContainer, void* SrcContainer)
{
	if (auto BoolProperty = CastField<FBoolProperty>(Property))
	{
		if (Property->GetMetaDataMap()->Contains("InlineEditConditionToggle") &&
			Property->NamePrivate.ToString().StartsWith("Override_"))
		{
			if (const bool Value = BoolProperty->GetPropertyValue_InContainer(SrcContainer))
			{
				BoolProperty->SetPropertyValue_InContainer(DstContainer, Value);
			}
		}
		else
		{
			//Property.
		}
	}
}
