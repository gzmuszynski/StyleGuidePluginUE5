// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetNaming//SGAssetNamingTypeSettings.h"


FSGAssetNamingTypeSettings::FSGAssetNamingTypeSettings(): ValidationVerbosity(ESGValidationVerbosity::Error)
{
	Variants.Add(FSGAssetNamingTypeVariant(FName("Default")));
}

bool FSGAssetNamingTypeSettings::operator==(const FSGAssetNamingTypeSettings& Other) const
{
	return Other.Class->IsChildOf(Class.Get());
}

bool FSGAssetNamingTypeSettings::operator==(const TSoftClassPtr<>& Other) const
{
	return Class->IsChildOf(Other.Get());
}

FSGAssetNamingTypeVariant& FSGAssetNamingTypeSettings::GetDefaultVariant()
{
	return Variants[0];
}

bool FSGAssetNamingTypeTable::Contains(const TSoftClassPtr<UObject>& Type) const
{
	return Types.Contains(Type);
}

FSGAssetNamingTypeSettings* FSGAssetNamingTypeTable::Find(const TSoftClassPtr<UObject>& Type, const UObject* Object) const
{
	if (Type.IsValid())
	{
		FSGAssetNamingTypeSettings *TypeSettings, *TypeSettingsBlueprint = nullptr;
		UClass* Class = Type.Get();
		do
		{
			auto Predicate = [Class](const FSGAssetNamingTypeSettings& Setting)
			{
				UClass* ThisClass = Setting.Class.Get();
				return ThisClass == Class;
			};
			
			TypeSettings = const_cast<FSGAssetNamingTypeSettings*>(Types.FindByPredicate(Predicate));
			if (Class == UBlueprint::StaticClass())
			{
				TypeSettingsBlueprint = TypeSettings;
				TypeSettings = nullptr;
				if (auto Blueprint = Cast<UBlueprint>(Object))
				{
					Class = Blueprint->ParentClass;
				}
				else
				{
					Class = Class->GetSuperClass();
				}
			}
			else
			{
				Class = Class->GetSuperClass();
			}
		} while (!TypeSettings && Class != nullptr && Object);
		return !TypeSettings && TypeSettingsBlueprint? TypeSettingsBlueprint : TypeSettings;
	}
	return {};
}

void FSGAssetNamingTypeTable::Add(const FSGAssetNamingTypeSettings& InTypeSettings)
{
	Types.Add(InTypeSettings);
}
