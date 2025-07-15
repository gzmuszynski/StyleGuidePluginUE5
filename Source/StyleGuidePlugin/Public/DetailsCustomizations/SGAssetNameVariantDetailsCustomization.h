// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"

/**
	* 
	*/

class FSGAssetNameVariantDetailsCustomization : public IPropertyTypeCustomization
{
public:
	static TSharedRef<IPropertyTypeCustomization> MakeInstance();

	virtual void CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle, FDetailWidgetRow& HeaderRow,
			IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	virtual void CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle, IDetailChildrenBuilder& ChildBuilder,
	 IPropertyTypeCustomizationUtils& CustomizationUtils) override;

	static TSharedRef<SWidget> MakeVariantValueWidget(TSharedRef<IPropertyHandle> VariantHandle,
	            IPropertyTypeCustomizationUtils& CustomizationUtils, TSharedPtr<IPropertyHandle> DefaultVariant = {});

	TSharedPtr<IPropertyHandle> PropertyHandle;
	TSharedPtr<IPropertyHandle> DefaultVariantHandle;
};
