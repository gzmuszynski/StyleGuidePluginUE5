// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SGValidationEnums.h"
#include "Engine/DeveloperSettings.h"
#include "SGDeveloperSettings.generated.h"

/**
 * 
 */


class USGSetup;

UCLASS(Config=Editor, DefaultConfig, meta=(DisplayName="Style Guide"))
class STYLEGUIDECORE_API USGDeveloperSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//What highest verbosity level should failed validation checks be classified as.
	//Any Module verbosity levels will be reduced to match this setting.
	UPROPERTY(EditAnywhere, Config, Category="Global")
	ESGValidationVerbosity HighestNameValidationFailedVerbosity = ESGValidationVerbosity::Warning;

	//This setup asset will be enforced globally, as lowest priority setup, regardless of directory hierarchy.
	//WARNING: this will apply to all plugins inside project directory. However, this can be a way to enforce
	//Style Guide for DLC and game features modules.
	UPROPERTY(EditAnywhere, Config, Category="Global")
	TSoftObjectPtr<USGSetup> GlobalSetup;
};
