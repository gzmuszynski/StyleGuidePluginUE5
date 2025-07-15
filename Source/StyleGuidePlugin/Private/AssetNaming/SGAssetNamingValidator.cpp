// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetNaming/SGAssetNamingValidator.h"

#include "AssetNaming/SGAssetNamingModule.h"
#include "Core/SGModule.h"
#include "Engine/AssetManager.h"
#include "Core/SGSetup.h"

#define LOCTEXT_NAMESPACE "StyleGuideNamingScheme"

#define ConditionalSet(FieldName) if (Module->Settings.Override_##FieldName) \
			{ Settings.##FieldName = Module->Settings.##FieldName;}

#define ConditionalGet(FieldName) AssetNamingSettings.##FieldName != ESGValidationVerbosity::None \
	&& AssetNamingSettings.Override_##FieldName

FSGAssetNamingSettings USGAssetNamingValidator::GetAssetNamingSettings(const FAssetData& AssetData) const
{
	FSGAssetNamingSettings Settings = {};
	if(auto SGDeveloperSettings = GetMutableDefault<USGDeveloperSettings>())
	{
		if(USGSetup* Setup = SGDeveloperSettings->GlobalSetup.Get())
		{
			if (auto AssetNamingModule = Setup->FindModuleByClass<USGAssetNamingModule>())
			{
				Settings = AssetNamingModule->Settings;
			}
			Settings.AllowedCharacters = Setup->Settings.AllowedCharacters;
		}
	}
	for(const USGSetup* Setup : GetStyleGuideSetupAssets(AssetData))
	{
		if (Setup->Settings.Override_AllowedCharacters)
		{
			Settings.AllowedCharacters = Setup->Settings.AllowedCharacters;
		}
		if (auto Module = Setup->FindModuleByClass<USGAssetNamingModule>())
		{
			ConditionalSet(IdentifierCheckVerbosity);
			ConditionalSet(UnknownTypesVerbosity);
			ConditionalSet(PreSuffixVerbosity);
			ConditionalSet(AllowedCharacters);
			
			for (const FSGAssetNamingTypeSettings& ModuleTypeSettings : Module->Settings.AssetTypeTable.Types)
			{
				Settings.OverrideTypeSettings(ModuleTypeSettings);
			}
		}
	}
	return Settings;
}

ESGValidationVerbosity USGAssetNamingValidator::GetValidationFailedVerbosity_Implementation(const FAssetData& Asset) const
{
	const FSGAssetNamingSettings Settings = GetAssetNamingSettings(Asset);
	return Settings.IdentifierCheckVerbosity;
}

bool USGAssetNamingValidator::CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
                                                            FDataValidationContext& InContext) const
{
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

EDataValidationResult USGAssetNamingValidator::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData,
	UObject* InAsset, FDataValidationContext& Context)
{
	Super::ValidateLoadedAsset_Implementation(InAssetData, InAsset, Context);
	
	FSGAssetNamingSettings AssetNamingSettings = GetAssetNamingSettings(InAssetData);

	FSGAssetNamingTypeSettings* TypeSettings = AssetNamingSettings.AssetTypeTable.Find(InAssetData.GetClass());
	
	FString AssetName = InAssetData.AssetName.ToString();
	

	
	if(ConditionalGet(IdentifierCheckVerbosity))
	{
		if (Result == EDataValidationResult::NotValidated)
		{
			Result = EDataValidationResult::Valid;
		}
			FRegexPattern Pattern(AssetNamingSettings.AllowedCharacters);
			FRegexMatcher Matcher(Pattern, AssetName);
			if(Matcher.FindNext())
			{
				int MatchBegin = Matcher.GetMatchBeginning();
				int MatchEnd = Matcher.GetMatchEnding();
				int NameLength = AssetName.Len();
			
				if(MatchBegin != 0 || MatchEnd != NameLength)
				{
					FFormatNamedArguments Args;
					Args.Add(TEXT("AssetName"), FText::FromString(AssetName));

					Result = SubmitValidationFailEvent(AssetNamingSettings.IdentifierCheckVerbosity, InAsset,
					FText::Format(LOCTEXT("FailTest", "Asset Contains illegal characters in the filename"), Args));
				}
			}
	}
	if (!TypeSettings || TypeSettings->Variants.IsEmpty())
	{
		if (ConditionalGet(UnknownTypesVerbosity))
		{
			Result = EDataValidationResult::Valid;
			FFormatNamedArguments Args;
			Args.Add(TEXT("AssetName"), FText::FromString(AssetName));
			Result = SubmitValidationFailEvent(AssetNamingSettings.UnknownTypesVerbosity, InAsset,
					FText::Format(LOCTEXT("FailTest", "Asset has an unknown class"), Args));
		}
	}
	else if(ConditionalGet(PreSuffixVerbosity))
    {
		const FSGAssetNamingTypeVariant& DefaultVariant = TypeSettings->Variants[0];
		
        if (Result == EDataValidationResult::NotValidated)
        {
            Result = EDataValidationResult::Valid;
        }
        for(const FSGAssetNamingTypeVariant& Variant : TypeSettings->Variants)
        {
            bool PrefixCorrect;
            bool SuffixCorrect;
 
            if(Variant.Prefix.IsEmpty())
            {
             	if (DefaultVariant.Prefix.IsEmpty())
             	{
             		PrefixCorrect = true;
             	}
             	else
             	{
             		PrefixCorrect = AssetName.StartsWith(DefaultVariant.Prefix);
             	}
            }
            else
            {
             	PrefixCorrect = AssetName.StartsWith(Variant.Prefix);
            }
 
            if(Variant.Suffix.IsEmpty())
            {
             	if (DefaultVariant.Suffix.IsEmpty())
             	{
             		SuffixCorrect = true;
             	}
             	else
             	{
             		SuffixCorrect = AssetName.StartsWith(DefaultVariant.Suffix);
             	}
            }
            else
            {
             	SuffixCorrect = AssetName.StartsWith(Variant.Suffix);
            }
 
            if(!PrefixCorrect || !SuffixCorrect)
            {
             	FFormatNamedArguments Args;
             	Args.Add(TEXT("AssetName"), FText::FromString(AssetName));
             	Args.Add(TEXT("AssetType"), TypeSettings->Class->GetDisplayNameText());
             	
             	SubmitValidationFailEvent(AssetNamingSettings.PreSuffixVerbosity, InAsset,
             	FText::Format(LOCTEXT("FailTest", "Asset does not follow asset naming scheme for asset type {AssetType}"), Args));
            }
        }
    }
	return Result;
}
#undef LOCTEXT_NAMESPACE