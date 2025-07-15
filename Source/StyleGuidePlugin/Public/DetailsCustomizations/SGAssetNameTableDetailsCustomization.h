// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AssetNaming/SGAssetNamingTypeSettings.h"
#include "UObject/Object.h"

/**
 * 
 */

class FSGAssetNameTableDetailsCustomization : public IPropertyTypeCustomization
{

public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow,
								 IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& ChildBuilder,
	                               IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	FSGAssetNamingTypeSettings NewEntry;

	TSharedPtr<IPropertyHandle> PropertyHandle;
};
