// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGAssetNamingTypeSettings.h"
#include "Core/SGModule.h"
#include "Core/SGValidationEnums.h"
#include "SGAssetNamingModule.generated.h"


USTRUCT(BlueprintType, DisplayName= "Style Guide Asset Naming Module")
struct STYLEGUIDEMODULES_API FSGAssetNamingSettings
{
	GENERATED_BODY()
	
	//Types will be validated according to these settings.
	//These will automatically add recursively, overriding only when same type has been defined in nested directory.
	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly)
	FSGAssetNamingTypeTable AssetTypeTable;

	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_IdentifierCheckVerbosity = true;

	// Defines the severity of failing the identifier check
	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(EditCondition="Override_IdentifierCheckVerbosity"))
	ESGValidationVerbosity IdentifierCheckVerbosity = ESGValidationVerbosity::Error;

	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_AllowedCharacters = false;
	
	// Defines the allowed characters during the identifier check.
	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(EditCondition="Override_AllowedCharacters"))
	FString AllowedCharacters = "[A-Za-z0-9_]+";

	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_UnknownTypesVerbosity = true;

	// Defines the severity of failing a check due to unknown type
	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(EditCondition="Override_UnknownTypesVerbosity"))
	ESGValidationVerbosity UnknownTypesVerbosity = ESGValidationVerbosity::Hint;

	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_PreSuffixVerbosity = true;

	// Defines the severity of failing a check due to unknown type
	UPROPERTY(EditAnywhere, Category="Asset Naming", BlueprintReadOnly, meta=(EditCondition="Override_PreSuffixVerbosity"))
	ESGValidationVerbosity PreSuffixVerbosity = ESGValidationVerbosity::Error;


	void OverrideTypeSettings(const FSGAssetNamingTypeSettings& InTypeSettings);
};

/**
 * 
 */
UCLASS(DisplayName="Asset Naming")
class STYLEGUIDEMODULES_API USGAssetNamingModule : public USGModule
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Asset Naming", meta=(ShowOnlyInnerProperties,
		EditCondition = "bDisableModuleChecks != true"))
	FSGAssetNamingSettings Settings;

	virtual void MergeModuleSettings_Implementation(USGModule* Module) override;

	virtual bool CanValidateAsset(const FAssetData& InAssetData, UObject* Object,
	                              FDataValidationContext& Context) const override;
	virtual EDataValidationResult ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
	                                                  const FDataValidationContext& Context) override;
};
