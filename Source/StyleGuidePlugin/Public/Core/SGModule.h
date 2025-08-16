// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "DataValidation/Public/EditorValidatorHelpers.h"
#include "SGModule.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct STYLEGUIDEPLUGIN_API FSGValidationMessage
{
	GENERATED_BODY()

	
	EMessageSeverity::Type Severity;

	UPROPERTY(BlueprintReadOnly)
	FText Text;
};

UCLASS(EditInlineNew, DefaultToInstanced, Abstract, DisplayName="Style Guide Module")
class STYLEGUIDEPLUGIN_API USGModule : public UObject
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintNativeEvent, Category="Style Guide Module")
	void MergeModuleSettings(USGModule* Module);

	UFUNCTION()
	TArray<FSGValidationMessage> GetValidationMessages();
	
	virtual bool CanValidateAsset(const FAssetData& AssetData, UObject* Object, FDataValidationContext& Context) const;
	virtual EDataValidationResult ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
	                                                  const FDataValidationContext& Context);

protected:

	//This override will disable any module checks, regardless of higher-level style guide setups.
	UPROPERTY(EditAnywhere, Category="Module", BlueprintReadOnly)
	bool bDisableModuleChecks = false;
	
	bool IsCheckEnabled(FName CheckPropertyName) const;
	//Automatically checks asset validation fail verbosity, adds message to context stack,
	//And returns appropriate validation result.
	UFUNCTION(BlueprintCallable, meta=(BlueprintProtected))
	EDataValidationResult SubmitValidationFailEvent(ESGValidationVerbosity Verbosity, const UObject* Asset, FText Message);

	UPROPERTY(BlueprintReadOnly)
	TArray<FSGValidationMessage> ValidationMessages;

	UPROPERTY(BlueprintReadOnly)
	EDataValidationResult Result = EDataValidationResult::NotValidated;
};
