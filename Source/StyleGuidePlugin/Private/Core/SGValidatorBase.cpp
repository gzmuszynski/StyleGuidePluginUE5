// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGValidatorBase.h"

#include "AssetRegistry/AssetRegistryModule.h"
#include "Core/SGModule.h"
#include "Core/SGSetup.h"

USGModule* USGValidatorBase::GetModuleSettings(const TSubclassOf<USGModule>& ModuleClass, const FAssetData& AssetData) const
{
	USGModule* ResultModule = NewObject<USGModule>(AssetData.FastGetAsset(true), ModuleClass);
	TArray<USGSetup*> Setups;
	if(auto SGDeveloperSettings = GetMutableDefault<USGDeveloperSettings>())
	{
		if(USGSetup* Setup = SGDeveloperSettings->GlobalSetup.Get())
		{
			Setups.Add(Setup);
		}
	}
	Setups.Append( GetStyleGuideSetupAssets(AssetData));
	for(const USGSetup* Setup : Setups)
	{
		for(USGModule* Module : Setup->Settings.Modules)
		{
			if(Module->GetClass() == ModuleClass)
			{
				CaptureProperties(ModuleClass->PropertyLink, Module, ResultModule);
			}
		}
	}
	return ResultModule;
}

ESGValidationVerbosity USGValidatorBase::GetValidationFailedVerbosity_Implementation(const FAssetData& Asset) const
{
	return ESGValidationVerbosity::None;
}

void USGValidatorBase::CaptureProperties(const FProperty* PropertyLink, const void* SrcContainer, void* DstContainer)
{	
	while(PropertyLink)
	{
		const void* Src = PropertyLink->ContainerPtrToValuePtr<void>(SrcContainer);
		void* Dst = PropertyLink->ContainerPtrToValuePtr<void>(DstContainer);
		
		if(Src && Dst)
		{
			if(auto StructProperty = CastField<FStructProperty>(PropertyLink))
			{
				FProperty* InnerProperty = StructProperty->Struct->PropertyLink;
				const void* InnerSrc = InnerProperty->ContainerPtrToValuePtr<void>(Src);
				void* InnerDst = InnerProperty->ContainerPtrToValuePtr<void>(Dst);
			
				CaptureProperties(InnerProperty, InnerSrc, InnerDst);
			}
			else if(auto ArrayProperty = CastField<FArrayProperty>(PropertyLink))
			{
				FScriptArrayHelper HelperSrc(ArrayProperty, Src);
				FScriptArrayHelper HelperDst(ArrayProperty, Dst);
				const FProperty* InnerProperty = ArrayProperty->Inner;
				for(int X = 0; X < HelperSrc.Num(); X++)
				{
					void* ElementSrcPtr = HelperSrc.GetElementPtr(X);
					bool bWasFound = false;
					for(int Y = 0; Y < HelperDst.Num(); Y++)
					{
						void* ElementDstPtr = HelperDst.GetElementPtr(Y);
						if(InnerProperty->Identical(ElementSrcPtr, ElementDstPtr))
						{
							bWasFound = true;
							ElementSrcPtr = HelperSrc.GetElementPtr(X);
							CaptureProperties(InnerProperty, ElementSrcPtr, ElementDstPtr);
						}
					}
					if(!bWasFound)
					{
						int NewIndex = HelperDst.AddValue();
						ElementSrcPtr = HelperSrc.GetElementPtr(X);
						void* ElementDstPtr = HelperDst.GetElementPtr(NewIndex);
						CaptureProperties(InnerProperty, ElementSrcPtr, ElementDstPtr);
					}
				}
			}
			else if(auto MapProperty = CastField<FMapProperty>(PropertyLink))
			{
				FScriptMapHelper HelperSrc(MapProperty, Src);
				FScriptMapHelper HelperDst(MapProperty, Dst);

				const FProperty* KeyProperty = MapProperty->GetKeyProperty();
				const FProperty* ValueProperty = MapProperty->GetValueProperty();
			
				for(int X = 0; X < HelperSrc.Num(); X++)
				{
					void* KeySrcPtr = HelperSrc.GetKeyPtr(X);
					void* ValueSrcPtr = HelperSrc.GetValuePtr(X);
				
					bool bWasFound = false;
					for(int Y = 0; Y < HelperDst.Num(); Y++)
					{
						void* KeyDstPtr = HelperDst.GetKeyPtr(Y);
						void* ValueDstPtr = HelperDst.GetValuePtr(Y);
						if(KeyProperty->Identical(KeySrcPtr, KeyDstPtr))
						{
							KeySrcPtr = HelperSrc.GetKeyPtr(X);
							ValueSrcPtr = HelperSrc.GetValuePtr(X);
							CaptureProperties(ValueProperty, ValueSrcPtr, ValueDstPtr);
							bWasFound = true;
						}
					}
					if(!bWasFound)
					{
						HelperDst.AddPair(KeySrcPtr, ValueSrcPtr);
					}
				}
			}
			else
			{
				PropertyLink->CopyCompleteValue(Dst,Src);
			}
		}
		PropertyLink = PropertyLink->PropertyLinkNext;
	}
}

EDataValidationResult USGValidatorBase::ValidateLoadedAsset_Implementation(const FAssetData& InAssetData,
	UObject* InAsset, FDataValidationContext& Context)
{
	CachedSettings = {};
	for(auto Setup : GetStyleGuideSetupAssets(InAssetData))
	{
		FProperty* PropertyLink = CachedSettings.StaticStruct()->PropertyLink;
		CaptureProperties(PropertyLink, &Setup->Settings, &CachedSettings);
	}
	return Super::ValidateLoadedAsset_Implementation(InAssetData, InAsset, Context);
}

TArray<USGSetup*> USGValidatorBase::GetStyleGuideSetupAssets(const FAssetData& InAssetData) const
{
	TArray<USGSetup*> SGSetups;
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();

	TArray<FAssetData> RedirectAssets;
	AssetRegistry.GetAssetsByClass(USGSetup::StaticClass()->GetClassPathName(), RedirectAssets);
	for(const FAssetData& AssetData : RedirectAssets)
	{
		if(InAssetData.PackagePath == AssetData.PackagePath ||
			InAssetData.PackagePath.ToString().Contains(AssetData.PackagePath.ToString()))
		{
			SGSetups.Add(Cast<USGSetup>(AssetData.GetAsset()));
		}
	}
	return SGSetups;
}

EDataValidationResult USGValidatorBase::SubmitValidationFailEvent(
	const ESGValidationVerbosity Verbosity, const UObject* Asset, FText Message)
{
	switch (Verbosity)
	{
	case ESGValidationVerbosity::None:
		{
			break;
		}
	case ESGValidationVerbosity::Hint:
		{
			AssetMessage(Asset, EMessageSeverity::Info, Message);
			break;
		}
	case ESGValidationVerbosity::Warning:
		{
			AssetWarning(Asset, Message);
			break;
		}
	case ESGValidationVerbosity::Error:
		{
			Result = EDataValidationResult::Invalid;
			AssetFails(Asset,Message);
			return Result;
		}
	}
	if (Result == EDataValidationResult::NotValidated)
	{
		AssetPasses(Asset);
	}
	return Result;
}
