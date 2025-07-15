// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailsCustomizations/SGAssetNameVariantDetailsCustomization.h"

#include "DetailWidgetRow.h"
#include "IPropertyUtilities.h"
#include "PropertyCustomizationHelpers.h"
#include "AssetNaming/SGAssetNamingTypeSettings.h"

#define LOCTEXT_NAMESPACE "StyleGuideAssetNameVariant"

TSharedRef<IPropertyTypeCustomization> FSGAssetNameVariantDetailsCustomization::MakeInstance()
{
	return MakeShareable(new FSGAssetNameVariantDetailsCustomization());
}

void FSGAssetNameVariantDetailsCustomization::CustomizeHeader(TSharedRef<IPropertyHandle> InPropertyHandle,
	FDetailWidgetRow& HeaderRow, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
	PropertyHandle = InPropertyHandle;
	DefaultVariantHandle = nullptr;
	if(PropertyHandle->GetParentHandle().IsValid() && PropertyHandle->GetParentHandle()->AsArray().IsValid())
	if (TSharedRef<IPropertyHandle> DefaultHandle = PropertyHandle->GetParentHandle()->AsArray()->GetElement(0);
		DefaultHandle != PropertyHandle)
	{
		DefaultVariantHandle = DefaultHandle;
	}
	HeaderRow
	.NameContent()
	[
		SNew(SEditableTextBox)
		.HintText(LOCTEXT("AssetVariantHint", "Asset Variant"))
		.Text_Lambda([this]()->FText
		{
			FText OutText;
			PropertyHandle->GetChildHandle(
				GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Name))->GetValueAsDisplayText(OutText);
			return OutText;
		})
		.OnTextChanged_Lambda([InPropertyHandle](const FText& InText)
		{
			InPropertyHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Name))
			->SetValueFromFormattedString(InText.ToString());
		})
	]
	.ValueContent()
	[
		MakeVariantValueWidget(InPropertyHandle, CustomizationUtils, DefaultVariantHandle)
	];
}

void FSGAssetNameVariantDetailsCustomization::CustomizeChildren(TSharedRef<IPropertyHandle> InPropertyHandle,
	IDetailChildrenBuilder& ChildBuilder, IPropertyTypeCustomizationUtils& CustomizationUtils)
{
}

TSharedRef<SWidget> FSGAssetNameVariantDetailsCustomization::MakeVariantValueWidget(
	TSharedRef<IPropertyHandle> VariantHandle, IPropertyTypeCustomizationUtils
	& CustomizationUtils, TSharedPtr<IPropertyHandle> DefaultVariant)
{
	auto HorizontalBox = SNew(SHorizontalBox)
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SEditableTextBox)
			.Font(CustomizationUtils.GetRegularFont())
			.Padding(FMargin(10.f,2.f,-5.f,2.f))
			.BackgroundColor(FSlateColor(FColor::Transparent))
			.Text_Lambda([VariantHandle]()->FText
			{
				if(VariantHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Prefix)) )
				{
					FText OutText;
				   VariantHandle->GetChildHandle(
					   GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Prefix))->GetValueAsDisplayText(OutText);
				   return OutText;
				}
				return FText();
			})
			.HintText_Lambda([VariantHandle, DefaultVariant]()->FText
			{
				if(!DefaultVariant.IsValid() || DefaultVariant == VariantHandle)
				{
					return LOCTEXT("Prefix", "Prefix");
				}
				FText OutText;
				if(DefaultVariant.Get() && DefaultVariant->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Prefix)))
				DefaultVariant->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Prefix))
				->GetValueAsDisplayText(OutText);
				return OutText;
			})
			.ToolTipText(LOCTEXT("PrefixTooltip", "Prefix"))
			.OnTextChanged_Lambda([VariantHandle](const FText& InText)
			{
				VariantHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Prefix))
				->SetValueFromFormattedString(InText.ToString());
			})
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Format", "Name_Variant"))
			.Font(CustomizationUtils.GetRegularFont())
		]
		+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SEditableTextBox)
			.Font(CustomizationUtils.GetRegularFont())
			.Padding(FMargin(1.f,2.f,5.f,2.f))
			.BackgroundColor(FSlateColor(FColor::Transparent))
			.Text_Lambda([VariantHandle]()->FText
			{
				if(VariantHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Prefix)) )
				{
					FText OutText;
					VariantHandle->GetChildHandle(
						GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Suffix))->GetValueAsDisplayText(OutText);
					return OutText;
				}
				return FText();
			})
			.HintText_Lambda([VariantHandle, DefaultVariant]()->FText
			{
				if(!DefaultVariant.IsValid() || DefaultVariant == VariantHandle)
				{
					return LOCTEXT("Suffix", "Suffix");
				}
				FText OutText;
				if(DefaultVariant.Get() && DefaultVariant->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Suffix)))
				DefaultVariant->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Suffix))
				->GetValueAsDisplayText(OutText);
				return OutText;
			})
			.ToolTipText(LOCTEXT("SuffixTooltip", "Suffix"))
			.OnTextChanged_Lambda([VariantHandle](const FText& InText)
			{
				VariantHandle->GetChildHandle(GET_MEMBER_NAME_CHECKED(FSGAssetNamingTypeVariant, Suffix))
				->SetValueFromFormattedString(InText.ToString());
			})
		];
		if(!DefaultVariant || VariantHandle != DefaultVariant)
		{
			HorizontalBox->AddSlot()
		   .FillWidth(1.0)
		   .HAlign(HAlign_Center)
		   [
			   SNew(SSpacer)
		   ];
			HorizontalBox->AddSlot()
			.AutoWidth()
			.HAlign(HAlign_Right)
			[
			PropertyCustomizationHelpers::MakeInsertDeleteDuplicateButton(
				FExecuteAction::CreateLambda(
				[ HorizontalBox,VariantHandle, &CustomizationUtils]()
				{
					if(TSharedPtr<IPropertyHandleArray> Array = VariantHandle->GetParentHandle()->AsArray())
					{
						HorizontalBox->SetVisibility(EVisibility::Collapsed);
						uint32 Index = VariantHandle->GetIndexInArray();
						VariantHandle->GetParentHandle()->AsArray()->Insert(Index);
						VariantHandle->GetParentHandle()->RequestRebuildChildren();
						CustomizationUtils.GetPropertyUtilities()->ForceRefresh();
					}
				}),
				FExecuteAction::CreateLambda(
				[ HorizontalBox,VariantHandle, &CustomizationUtils]()
				{
					if(TSharedPtr<IPropertyHandleArray> Array = VariantHandle->GetParentHandle()->AsArray())
					{
						HorizontalBox->SetVisibility(EVisibility::Collapsed);
						uint32 Index = VariantHandle->GetIndexInArray();
						VariantHandle->GetParentHandle()->AsArray()->DeleteItem(Index);
						VariantHandle->GetParentHandle()->RequestRebuildChildren();
						CustomizationUtils.GetPropertyUtilities()->ForceRefresh();
					}
				}),FExecuteAction::CreateLambda(
				[ HorizontalBox,VariantHandle, &CustomizationUtils]()
				{
					if(TSharedPtr<IPropertyHandleArray> Array = VariantHandle->GetParentHandle()->AsArray())
					{
						HorizontalBox->SetVisibility(EVisibility::Collapsed);
						uint32 Index = VariantHandle->GetIndexInArray();
						VariantHandle->GetParentHandle()->AsArray()->DuplicateItem(Index);
						VariantHandle->GetParentHandle()->RequestRebuildChildren();
						CustomizationUtils.GetPropertyUtilities()->ForceRefresh();
					}
				}))
			];
		}
	return HorizontalBox;
}