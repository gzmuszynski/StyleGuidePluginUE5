// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "SGModule.h"
#include "Core/SGSettings.h"
#include "Settings/SGDeveloperSettings.h"
#include "SGValidatorBase.generated.h"

class USGSetup;
/**
 * An abstract base class that takes advantage of Style Guide framework.
 */
UCLASS(Abstract, DisplayName="Style Guide Validator Base")
class STYLEGUIDEPLUGIN_API USGValidatorBase : public UEditorValidatorBase
{
public:
	template<class T>
	T* GetModuleSettings(const FAssetData& AssetData) const
	{
		if(T::StaticClass().IsChildOf(USGModule::StaticClass()))
		{
			return GetModuleSettings(T::GetClass(), AssetData);
		}
		return nullptr;
	}
	USGModule* GetModuleSettings(const TSubclassOf<USGModule>& ModuleClass, const FAssetData& AssetData) const;
	static void CaptureProperties(const FProperty* PropertyLink, const void* Src, void* Dst);
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
	                                                                 FDataValidationContext& Context) override;

private:
	GENERATED_BODY()

public:

protected:
	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	TArray<USGSetup*> GetStyleGuideSetupAssets(const FAssetData& InAssetData) const;
	
	//Determines verbosity of asset validation fail events.
	//A custom-made validator should always override this function!
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, meta=(BlueprintProtected))
	ESGValidationVerbosity GetValidationFailedVerbosity(const FAssetData& Asset) const;

	//Automatically checks asset validation fail verbosity, adds message to context stack,
	//And returns appropriate validation result.
	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	EDataValidationResult SubmitValidationFailEvent(ESGValidationVerbosity Verbosity, const UObject* Asset, FText Message);

	UPROPERTY(BlueprintReadOnly)
	EDataValidationResult Result = EDataValidationResult::NotValidated;
	
	UPROPERTY(BlueprintReadOnly)
	FSGSettings CachedSettings;
};
