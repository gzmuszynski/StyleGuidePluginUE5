// Fill out your copyright notice in the Description page of Project Settings.


#include "AssetNaming/SGAssetNamingModule.h"

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
