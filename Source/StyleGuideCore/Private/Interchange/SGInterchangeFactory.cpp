// Fill out your copyright notice in the Description page of Project Settings.


#include "Interchange/SGInterchangeFactory.h"

#include "Core/SGSetup.h"
#include "Interchange/SGInterchangeFactoryNode.h"

UClass* USGInterchangeFactory::GetFactoryClass() const
{
	return USGSetup::StaticClass();
}

UInterchangeFactoryBase::FImportAssetResult USGInterchangeFactory::BeginImportAsset_GameThread(
	const FImportAssetObjectParams& Arguments)
{
	USGSetup* StyleGuideSetup = nullptr;
	FImportAssetResult ImportAssetResult;
	
	if (!Arguments.AssetNode || !Arguments.AssetNode->GetObjectClass()->IsChildOf(GetFactoryClass()))
	{
		return ImportAssetResult;
	}

	USGInterchangeFactoryNode* SGNode = Cast<USGInterchangeFactoryNode>(Arguments.AssetNode);
	if (SGNode == nullptr)
	{
		return ImportAssetResult;
	}

	bool bIsReImport = Arguments.ReimportObject != nullptr;
	
	UObject* ExistingAsset = Arguments.ReimportObject;
	if (!ExistingAsset)
	{
		FSoftObjectPath ReferenceObject;
		/*if (SGNode->GetCustomReferenceObject(ReferenceObject))
		{
			ExistingAsset = ReferenceObject.TryLoad();
		}*/
	}

	// create a new skeleton or overwrite existing asset, if possible
	if (!ExistingAsset)
	{
		StyleGuideSetup = NewObject<USGSetup>(Arguments.Parent, USGSetup::StaticClass(), *Arguments.AssetName, RF_Public | RF_Standalone);
	}
	else
	{
		//This is a reimport, we are just re-updating the source data
		StyleGuideSetup = Cast<USGSetup>(ExistingAsset);
	}
	ImportAssetResult.ImportedObject = StyleGuideSetup;
	return ImportAssetResult;
}
