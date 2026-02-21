// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterchangeTranslatorBase.h"
#include "SGInterchangeTranslator.generated.h"

/**
 * 
 */
UCLASS()
class STYLEGUIDECORE_API USGInterchangeTranslator : public UInterchangeTranslatorBase
{
	GENERATED_BODY()

public:
	virtual bool CanImportSourceData(const UInterchangeSourceData* InSourceData) const override;
	virtual EInterchangeTranslatorType GetTranslatorType() const override;
	virtual EInterchangeTranslatorAssetType GetSupportedAssetTypes() const override;
	virtual TArray<FString> GetSupportedFormats() const override;
	virtual bool Translate(UInterchangeBaseNodeContainer& BaseNodeContainer) const override;
	virtual UInterchangeTranslatorSettings* GetSettings() const override;
	virtual void SetSettings(const UInterchangeTranslatorSettings* InterchangeTranslatorSettings) override;
};
