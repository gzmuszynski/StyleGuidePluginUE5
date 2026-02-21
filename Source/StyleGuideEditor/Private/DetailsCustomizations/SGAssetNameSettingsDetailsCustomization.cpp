// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailsCustomizations/SGAssetNameSettingsDetailsCustomization.h"

#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "SStandaloneCustomizedValueWidget.h"
#include "DetailsCustomizations/SGAssetNameVariantDetailsCustomization.h"
#include "AssetNaming/SGAssetNamingTypeSettings.h"

#define LOCTEXT_NAMESPACE "StyleGuideAssetNameDetails"

TSharedRef<IPropertyTypeCustomization> FSGAssetNameSettingsDetailsCustomization::MakeInstance()
{
	// Create the instance and returned a SharedRef
	return MakeShareable(new FSGAssetNameSettingsDetailsCustomization());
}

void FSGAssetNameSettingsDetailsCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle,
									FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	PropertyHandle = InPropertyHandle;
	FText ClassName;
	InPropertyHandle->GetChildHandle(
			GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, Class))->GetValueAsDisplayText(ClassName);

	TSharedPtr<IPropertyHandle> DefaultVariant = InPropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, Variants));
	if(DefaultVariant.IsValid() && DefaultVariant->AsArray())
	{
		DefaultVariant = DefaultVariant->AsArray()->GetElement(0);
	}
	else
	{
		DefaultVariant.Reset();
	}
	TSharedPtr<IPropertyHandle> Class = InPropertyHandle->GetChildHandle(
		GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, Class));
	
	HeaderRow
	.NameContent()
	[
		Class->CreatePropertyValueWidget()
	]
	.ValueContent()
	.MinDesiredWidth(250.f)
	.MaxDesiredWidth(0.f)
	[
		SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			FSGAssetNameVariantDetailsCustomization::MakeVariantValueWidget(
				DefaultVariant.ToSharedRef(), CustomizationUtils, DefaultVariant)
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SSpacer)
			.Size(FVector2D(1.f,10.f))
		]
		+ SHorizontalBox::Slot()
		.FillWidth(1.0)
		[
			PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, ValidationVerbosity))
			->CreatePropertyValueWidget()
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Right)
		[
			PropertyCustomizationHelpers::MakeAddButton(FSimpleDelegate::CreateLambda(
			[this, &CustomizationUtils]()
			{
				PropertyHandle->GetChildHandle(
					GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, Variants))->AsArray()->AddItem();
				PropertyHandle->GetParentHandle()->RequestRebuildChildren();
				CustomizationUtils.GetPropertyUtilities()->ForceRefresh();
			}), LOCTEXT("AddEntry", "Add New Entry for Asset Class"))
		]
	];
}

void FSGAssetNameSettingsDetailsCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
					IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	PropertyHandle = InPropertyHandle;
	uint32 NumChildren;
	InPropertyHandle->GetNumChildren(NumChildren);

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{
		TSharedRef<IPropertyHandle> ChildHandle = InPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		if(ChildHandle->GetPropertyDisplayName().ToString() ==
			GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, Variants))
		{
			uint32 NumVariants;
			
			TSharedPtr<IPropertyHandleArray> Array = 
				PropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeSettings, Variants))
				->AsArray();
			Array->GetNumElements(NumVariants);
			for(uint32 i = 1; i < NumVariants; i++)
			{
				ChildBuilder.AddProperty(Array->GetElement(i)).ShowPropertyButtons(false);
			}
		}
		else
		{
			//IDetailPropertyRow& Property = ChildBuilder.AddProperty(ChildHandle);
		}
	}
	
}