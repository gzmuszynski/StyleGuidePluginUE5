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

FSGAssetNamingTypeSettings* FSGAssetNamingTypeTable::Find(const TSoftClassPtr<UObject>& Type) const
{
	if (Type.IsValid())
	{
		return const_cast<FSGAssetNamingTypeSettings*>(Types.FindByPredicate(
		   [Type](const FSGAssetNamingTypeSettings& Setting)
		   {
			   return Setting==Type;
		   }));
	}
	return nullptr;
}

void FSGAssetNamingTypeTable::Add(const FSGAssetNamingTypeSettings& InTypeSettings)
{
	Types.Add(InTypeSettings);
}
