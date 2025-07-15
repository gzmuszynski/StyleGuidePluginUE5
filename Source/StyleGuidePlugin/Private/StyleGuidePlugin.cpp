// Copyright Epic Games, Inc. All Rights Reserved.

#include "StyleGuidePlugin.h"

#include "DetailsCustomizations/SGAssetNameSettingsDetailsCustomization.h"
#include "DetailsCustomizations/SGAssetNameTableDetailsCustomization.h"
#include "DetailsCustomizations/SGAssetNameVariantDetailsCustomization.h"
#include "AssetNaming/SGAssetNamingTypeSettings.h"

#define LOCTEXT_NAMESPACE "FStyleGuidePluginModule"

void FStyleGuidePluginModule::StartupModule()
{
	
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
	// to register our custom property
	/*PropertyModule.RegisterCustomPropertyTypeLayout(
		FSGAssetNameTable::StaticStruct()->GetFName(),
			FOnGetPropertyTypeCustomizationInstance::CreateStatic(
				&FSGAssetNameTableDetailsCustomization::MakeInstance));*/
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSGAssetNamingTypeSettings::StaticStruct()->GetFName(),
			FOnGetPropertyTypeCustomizationInstance::CreateStatic(
				&FSGAssetNameSettingsDetailsCustomization::MakeInstance));
	PropertyModule.RegisterCustomPropertyTypeLayout(
		FSGAssetNamingTypeVariant::StaticStruct()->GetFName(),
			FOnGetPropertyTypeCustomizationInstance::CreateStatic(
				&FSGAssetNameVariantDetailsCustomization::MakeInstance));

	PropertyModule.NotifyCustomizationModuleChanged();
}

void FStyleGuidePluginModule::ShutdownModule()
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		// unregister properties when the module is shutdown
		FPropertyEditorModule& PropertyModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSGAssetNamingTypeTable::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSGAssetNamingTypeSettings::StaticStruct()->GetFName());
		PropertyModule.UnregisterCustomPropertyTypeLayout(FSGAssetNamingTypeVariant::StaticStruct()->GetFName());

		PropertyModule.NotifyCustomizationModuleChanged();
	}
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStyleGuidePluginModule, StyleGuidePlugin)