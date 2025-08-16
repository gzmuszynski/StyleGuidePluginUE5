// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetNaming/SGAssetNamingModule.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "AssetRegistry/IAssetRegistry.h"
#include "Core/SGSetup.h"

#define LOCTEXT_NAMESPACE "StyleGuideNamingScheme"

#define ConditionalSet(FieldName) if (AssetNamingModule->Settings.Override_##FieldName) \
{ Settings.FieldName = AssetNamingModule->Settings.FieldName;}

void FSGAssetNamingSettings::OverrideTypeSettings(const FSGAssetNamingTypeSettings& InTypeSettings)
{
	if (FSGAssetNamingTypeSettings* TypeSettings = AssetTypeTable.Find(InTypeSettings.Class))
	{
		TypeSettings->Variants = InTypeSettings.Variants;
	}
	else
	{
		AssetTypeTable.Add(InTypeSettings);
	}
}

void USGAssetNamingModule::MergeModuleSettings_Implementation(USGModule* Module)
{
	Super::MergeModuleSettings_Implementation(Module);
	
	if (const UClass* Class = GetClass(); Class == Module->GetClass())
	{
		if (auto AssetNamingModule = Cast<USGAssetNamingModule>(Module))
		{
			ConditionalSet(IdentifierCheckVerbosity);
			ConditionalSet(UnknownTypesVerbosity);
			ConditionalSet(PreSuffixVerbosity);
			ConditionalSet(AllowedCharacters);
		
			for (const FSGAssetNamingTypeSettings& ModuleTypeSettings : AssetNamingModule->Settings.AssetTypeTable.Types)
			{
				Settings.OverrideTypeSettings(ModuleTypeSettings);
			}
		}
	}
}

bool USGAssetNamingModule::CanValidateAsset(const FAssetData& InAssetData, UObject* Object,
                                            FDataValidationContext& Context) const
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

EDataValidationResult USGAssetNamingModule::ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
	const FDataValidationContext& Context)
{
	if (bDisableModuleChecks)
	{
		return EDataValidationResult::NotValidated;
	}
	
	FSGAssetNamingTypeSettings* TypeSettings = Settings.AssetTypeTable.Find(InAssetData.GetClass());
	
	FString AssetName = InAssetData.AssetName.ToString();
	
	if(IsCheckEnabled(GET_MEMBER_NAME_CHECKED(FSGAssetNamingSettings, IdentifierCheckVerbosity)))
	{
		if (Result == EDataValidationResult::NotValidated)
		{
			Result = EDataValidationResult::Valid;
		}
			FRegexPattern Pattern(Settings.AllowedCharacters);
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

					Result = SubmitValidationFailEvent(Settings.IdentifierCheckVerbosity, InAsset,
					FText::Format(LOCTEXT("FailTest", "Asset Contains illegal characters in the filename"), Args));
				}
			}
	}
	if (!TypeSettings || TypeSettings->Variants.IsEmpty())
	{
		if (IsCheckEnabled(GET_MEMBER_NAME_CHECKED(FSGAssetNamingSettings, UnknownTypesVerbosity)))
		{
			Result = EDataValidationResult::Valid;
			FFormatNamedArguments Args;
			Args.Add(TEXT("Class"), InAssetData.GetClass()->GetDisplayNameText());
			Result = SubmitValidationFailEvent(Settings.UnknownTypesVerbosity, InAsset,
					FText::Format(LOCTEXT("FailTest", "Asset uses class {Class}, which is unregistered with style guide"), Args));
		}
	}
	else if(IsCheckEnabled(GET_MEMBER_NAME_CHECKED(FSGAssetNamingSettings, PreSuffixVerbosity)))
    {
		const FSGAssetNamingTypeVariant& DefaultVariant = TypeSettings->Variants[0];
		
        if (Result == EDataValidationResult::NotValidated)
        {
            Result = EDataValidationResult::Valid;
        }
		bool PrefixCorrect = false;
        bool SuffixCorrect = false;
		
        for(const FSGAssetNamingTypeVariant& Variant : TypeSettings->Variants)
        {
            if(Variant.Prefix.IsEmpty())
            {
             	if (DefaultVariant.Prefix.IsEmpty())
             	{
             		PrefixCorrect = true;
             	}
             	else
             	{
             		PrefixCorrect |= AssetName.StartsWith(DefaultVariant.Prefix);
             	}
            }
            else
            {
             	PrefixCorrect |= AssetName.StartsWith(Variant.Prefix);
            }
 
            if(Variant.Suffix.IsEmpty())
            {
             	if (DefaultVariant.Suffix.IsEmpty())
             	{
             		SuffixCorrect = true;
             	}
             	else
             	{
             		SuffixCorrect |= AssetName.EndsWith(DefaultVariant.Suffix);
             	}
            }
            else
            {
             	SuffixCorrect |= AssetName.EndsWith(Variant.Suffix);
            }
        }
		
		if(!PrefixCorrect || !SuffixCorrect)
		{
			FFormatNamedArguments Args;
			Args.Add(TEXT("AssetName"), FText::FromString(AssetName));
			Args.Add(TEXT("AssetType"), TypeSettings->Class->GetDisplayNameText());

			SubmitValidationFailEvent(Settings.PreSuffixVerbosity, InAsset,
			FText::Format(LOCTEXT("FailTest", "Asset does not follow asset naming scheme for asset type {AssetType}"), Args));
		}
    }
	return Result;
}
#undef LOCTEXT_NAMESPACE