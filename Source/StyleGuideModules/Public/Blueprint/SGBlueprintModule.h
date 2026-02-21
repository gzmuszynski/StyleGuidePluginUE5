// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SGModule.h"
#include "Core/SGValidationEnums.h"
#include "SGBlueprintModule.generated.h"

/**
 * 
 */

USTRUCT(BlueprintType, DisplayName="Blueprint Module")
struct STYLEGUIDEMODULES_API FSGBlueprintSettings
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_IdentifierCheckVerbosity = true;

	// Defines the severity of failing the identifier check
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(EditCondition="Override_IdentifierCheckVerbosity"))
	ESGValidationVerbosity IdentifierCheckVerbosity = ESGValidationVerbosity::Error;

	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_AllowedCharacters = false;
	
	// Defines the allowed characters during the identifier check.
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(EditCondition="Override_AllowedCharacters"))
	FString AllowedCharacters = "[A-Za-z0-9_]+";

	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_CamelCaseCheckVerbosity = true;

	// Defines the severity of failing the CamelCase check
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(EditCondition="Override_CamelCaseCheckVerbosity"))
	ESGValidationVerbosity CamelCaseCheckVerbosity = ESGValidationVerbosity::Error;

	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_BoolPrefixCheckVerbosity = true;

	// Defines the severity of failing the CamelCase check
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(EditCondition="Override_BoolPrefixCheckVerbosity"))
	ESGValidationVerbosity BoolPrefixCheckVerbosity = ESGValidationVerbosity::Hint;
	
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_ReplicationPrefixCheckVerbosity = true;
	
	// Defines the severity of failing the Replicated events prefixes ("Server_", "Client_", "All_" check)
	UPROPERTY(EditAnywhere, Category="Blueprint Fields Naming", BlueprintReadOnly, meta=(EditCondition="Override_ReplicationPrefixCheckVerbosity"))
	ESGValidationVerbosity ReplicationPrefixCheckVerbosity = ESGValidationVerbosity::Warning;

	UPROPERTY(EditAnywhere, Category="Blueprint Size", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_SizeCheckVerbosity = true;

	// Defines the severity of failing the disk size check
	UPROPERTY(EditAnywhere, Category="Blueprint Size", BlueprintReadOnly, meta=(EditCondition="Override_SizeCheckVerbosity"))
	ESGValidationVerbosity SizeCheckVerbosity = ESGValidationVerbosity::Error;

	// Defines the max allowed disk size for blueprint types
	UPROPERTY(EditAnywhere, Category="Blueprint Size", BlueprintReadOnly,
		meta=(EditCondition="SizeCheckVerbosity!=ESGValidationVerbosity::None", EditConditionHides, ForceUnits="Megabytes"))
	int64 MaxAllowedSize = 64;
	
	UPROPERTY(EditAnywhere, Category="Blueprint Size", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_FunctionCountCheckVerbosity = true;

	// Defines the severity of failing the Function/Event Count check
	UPROPERTY(EditAnywhere, Category="Blueprint Size", BlueprintReadOnly, meta=(EditCondition="Override_FunctionCountCheckVerbosity"))
	ESGValidationVerbosity FunctionCountCheckVerbosity = ESGValidationVerbosity::Error;
	
	// Defines the max allowed function/event count for blueprint types
	UPROPERTY(EditAnywhere, Category="Blueprint Size", BlueprintReadOnly,
		meta=(EditCondition="FunctionCountCheckVerbosity!=ESGValidationVerbosity::None", EditConditionHides))
	int64 MaxAllowedFunctionCount = 50;
};

UCLASS(DisplayName="Blueprint")
class STYLEGUIDEMODULES_API USGBlueprintModule : public USGModule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category="Blueprint Settings", meta=(ShowOnlyInnerProperties))
	FSGBlueprintSettings Settings;
};
