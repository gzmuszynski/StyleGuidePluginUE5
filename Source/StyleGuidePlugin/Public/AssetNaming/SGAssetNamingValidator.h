// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAssetNamingModule.h"
#include "Core/SGValidatorBase.h"
#include "SGAssetNamingValidator.generated.h"

/**
 * 
 */
UCLASS()
class STYLEGUIDEPLUGIN_API USGAssetNamingValidator : public USGValidatorBase
{
	GENERATED_BODY()

public:
	FSGAssetNamingSettings GetAssetNamingSettings(const FAssetData& AssetData) const;
	virtual ESGValidationVerbosity GetValidationFailedVerbosity_Implementation(const FAssetData& Asset) const override;
	virtual bool CanValidateAsset_Implementation(const FAssetData& InAssetData, UObject* InObject,
	                                             FDataValidationContext& InContext) const override;
	virtual EDataValidationResult ValidateLoadedAsset_Implementation(const FAssetData& InAssetData, UObject* InAsset,
		FDataValidationContext& Context) override;
};
