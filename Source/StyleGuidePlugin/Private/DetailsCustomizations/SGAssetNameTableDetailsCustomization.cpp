// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailsCustomizations/SGAssetNameTableDetailsCustomization.h"

#include "DetailWidgetRow.h"
#include "IDetailChildrenBuilder.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "Behaviors/2DViewportBehaviorTargets.h"
#include "AssetNaming/SGAssetNamingTypeSettings.h"

#define LOCTEXT_NAMESPACE "StyleGuideAssetNameDetails"

TSharedRef<IPropertyTypeCustomization> FSGAssetNameTableDetailsCustomization::MakeInstance()
{
	// Create the instance and returned a SharedRef
	return MakeShareable(new FSGAssetNameTableDetailsCustomization());
}

void FSGAssetNameTableDetailsCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle,
						FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	auto ArrayProperty = CastField<FArrayProperty>(InPropertyHandle->GetProperty());
	
	if(true)
	{
		HeaderRow.
		NameContent()
		[
			SNew(STextBlock)
			.Font(CustomizationUtils.GetRegularFont())
			.Text(LOCTEXT("TableNameHeader", "Class"))
		]
		.ValueContent()
		[
			SNew(STextBlock)
			.Font(CustomizationUtils.GetRegularFont())
			.Text(LOCTEXT("TableValueHeader", "Naming Pattern and Verbosity"))
		];
	}
	else
	{
		HeaderRow.
		NameContent()
		[
			InPropertyHandle->CreatePropertyNameWidget()
		]
		.ValueContent()
		[
			InPropertyHandle->CreatePropertyValueWidget()
		];
	}
}

void FSGAssetNameTableDetailsCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
                        IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	uint32 NumChildren;
	InPropertyHandle->GetNumChildren(NumChildren);
	

	for (uint32 ChildIndex = 0; ChildIndex < NumChildren; ++ChildIndex)
	{		
		TSharedRef<IPropertyHandle> ChildHandle = InPropertyHandle->GetChildHandle(ChildIndex).ToSharedRef();
		if(ChildHandle->GetPropertyDisplayName().ToString() !=
			GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeTable, Types))
		{
			ChildBuilder.AddProperty(ChildHandle);
		}
		else
		{
			auto ArrayPropertyHandle = InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeTable, Types))
				.ToSharedRef()->AsArray();
			uint32 NumEntries;
			ArrayPropertyHandle->GetNumElements(NumEntries);
			for(uint32 i = 0; i < NumEntries; i++)
			{
				ChildBuilder.AddProperty(ArrayPropertyHandle->GetElement(i)).ShowPropertyButtons(false);
			}
	
			ChildBuilder.AddCustomRow(LOCTEXT("AddNewAssetNameSettings", "Add Asset Name Settings"))
			.NameContent()
			[
				SNew(SClassPropertyEntryBox)
				.MetaClass(UObject::StaticClass())
				.AllowNone(true)
				.SelectedClass_Lambda([this]()->UClass*
				{
					return NewEntry.Class.Get();
				})
				.OnSetClass_Lambda([this](const UClass* InClass)
				{
					NewEntry.Class = InClass;
				})
			]
			.ValueContent()
			.HAlign(HAlign_Fill)
			[
				SNew(SHorizontalBox)
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SEditableTextBox)
					.HintText(LOCTEXT("Prefix", "Prefix"))
					.ToolTipText(LOCTEXT("PrefixTooltip", "Prefix"))
					.OnTextChanged_Lambda([this](const FText& InText)
					{
						NewEntry.Variants[0].Prefix = InText.ToString();
					})
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Text(LOCTEXT("Format", "AssetName_Variant"))
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				[
					SNew(SEditableTextBox)
					.HintText(LOCTEXT("Suffix", "Suffix"))
					.ToolTipText(LOCTEXT("SuffixTooltip", "Suffix"))
					.OnTextChanged_Lambda([this](const FText& InText)
					{
						NewEntry.Variants[0].Suffix = InText.ToString();
					})
				]
				+ SHorizontalBox::Slot()
				.FillWidth(1.0)
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
				   SNew(SSpacer)
				]
				+ SHorizontalBox::Slot()
				.AutoWidth()
				.HAlign(HAlign_Right)
				[
					PropertyCustomizationHelpers::MakeAddButton(FSimpleDelegate::CreateLambda([this, &CustomizationUtils]	()
					{
						auto TypesArrayHandle = PropertyHandle->GetChildHandle(
						GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeTable, Types))->AsArray();
						if(TypesArrayHandle->AddItem())
						{
							uint32 NumElements;
							TypesArrayHandle->GetNumElements(NumElements);
							FString FormattedString;
							StaticStruct<FSGAssetNamingTypeSettings>()->ExportText(FormattedString, &NewEntry,
								&NewEntry, nullptr, PPF_None, nullptr, false);
							TypesArrayHandle.Get()->GetElement(NumElements-1)->SetValueFromFormattedString(FormattedString);
						}
						PropertyHandle->RequestRebuildChildren();
						CustomizationUtils.GetPropertyUtilities()->ForceRefresh();
					})
					, LOCTEXT("AddEntry", "Add New Entry for Asset Class"))
				]
			];
		}
	}
}
#undef LOCTEXT_NAMESPACE
