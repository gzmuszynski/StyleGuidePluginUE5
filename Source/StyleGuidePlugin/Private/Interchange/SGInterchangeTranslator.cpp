// Fill out your copyright notice in the Description page of Project Settings.


#include "Interchange/SGInterchangeTranslator.h"

#include "Interchange/SGInterchangeFactoryNode.h"
#include "Nodes/InterchangeSourceNode.h"

bool USGInterchangeTranslator::CanImportSourceData(const UInterchangeSourceData* InSourceData) const
{
	return Super::CanImportSourceData(InSourceData);
}

EInterchangeTranslatorType USGInterchangeTranslator::GetTranslatorType() const
{
	return EInterchangeTranslatorType::Assets;
}

EInterchangeTranslatorAssetType USGInterchangeTranslator::GetSupportedAssetTypes() const
{
	return EInterchangeTranslatorAssetType::None;
}

TArray<FString> USGInterchangeTranslator::GetSupportedFormats() const
{
	return { TEXT("md;Style Guide Markdown") };
}

bool USGInterchangeTranslator::Translate(UInterchangeBaseNodeContainer& BaseNodeContainer) const
{
	UInterchangeSourceNode* SourceNode = UInterchangeSourceNode::FindOrCreateUniqueInstance(&BaseNodeContainer);
	FString FileName = FPaths::GetBaseFilename(GetSourceData()->GetFilename());
	SourceNode->SetExtraInformation(TEXT("Filename"), FileName);
	SourceNode->SetExtraInformation(TEXT("Generator"), GetClass()->GetName());
	
	USGInterchangeFactoryNode* StyleGuideNode = nullptr;
	StyleGuideNode = NewObject<USGInterchangeFactoryNode>(&BaseNodeContainer);

	FString StyleGuideUid = TEXT("\\StyleGuide\\") + FileName;
	BaseNodeContainer.SetupNode(StyleGuideNode, StyleGuideUid, FileName, EInterchangeNodeContainerType::TranslatedAsset);

	//StyleGuideNode->SetCustomDisplayText(FileName);
	return true;
}

UInterchangeTranslatorSettings* USGInterchangeTranslator::GetSettings() const
{
	return Super::GetSettings();
}

void USGInterchangeTranslator::SetSettings(const UInterchangeTranslatorSettings* InterchangeTranslatorSettings)
{
	Super::SetSettings(InterchangeTranslatorSettings);
}
