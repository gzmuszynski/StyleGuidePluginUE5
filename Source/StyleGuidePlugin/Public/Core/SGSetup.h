// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SGSettings.h"
#include "Engine/DataAsset.h"
#include "SGSetup.generated.h"

/**
 * Style Guide Setup defines asset validation for all the assets in this directory, and all subdirectories recursively.
 * It can be overriden or appended (in the case of container properties) by placing a style guide setup in subdirectories.
 */
UCLASS(DisplayName="Style Guide Setup")
class STYLEGUIDEPLUGIN_API USGSetup : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category="Setup", meta=(ShowOnlyInnerProperties))
	FSGSettings Settings;
	
	UFUNCTION(BlueprintCallable, Category="Setup")
	void ImportStyleGuide();
	
	UFUNCTION(BlueprintCallable, Category="Setup")
	void ExportStyleGuide();
	
	UFUNCTION()
	void MergeSettings(USGSetup* Setup);

	template<typename ModuleType>
	ModuleType* FindModuleByClass() const
	{
		for (USGModule* Module : Settings.Modules)
		{
			if (ModuleType* ModuleCast = Cast<ModuleType>(Module))
			{
				return ModuleCast;
			}
		}
		return nullptr;
	}
};
