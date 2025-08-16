// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorValidatorBase.h"
#include "SGModule.h"
#include "Core/SGSettings.h"
#include "Settings/SGDeveloperSettings.h"
#include "SGValidator.generated.h"

class USGSetup;
/**
 * An abstract base class that takes advantage of Style Guide framework.
 */
UCLASS(DisplayName="Style Guide Validator")
class STYLEGUIDEPLUGIN_API USGValidator : public UEditorValidatorBase
{
	GENERATED_BODY()
public:
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
	                                                                 FDataValidationContext& Context) override;

	virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
		FDataValidationContext& InContext) const override;

protected:
	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	TArray<USGSetup*> GetStyleGuideSetupAssets(const FAssetData& InAssetData) const;
	
	
};
