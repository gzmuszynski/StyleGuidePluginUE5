// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SGSettings.generated.h"

class USGModule;
/**
 * 
 */
USTRUCT(BlueprintType)
struct STYLEGUIDEPLUGIN_API FSGSettings
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category="Common", BlueprintReadOnly, meta=(InlineEditConditionToggle))
	bool Override_AllowedCharacters = false;
	
	// Only allows identifiers that match following RegEx expression (can be overriden in modules)
	UPROPERTY(EditAnywhere, Category="Common", BlueprintReadOnly, meta=(EditCondition="Override_AllowedCharacters"))
	FString AllowedCharacters = "[A-Za-z0-9_]+";
	
	// Assets from within those directories will be excluded from style guide validation.
	// This paths are relative to this asset's directory, no partial, nested match works.
	UPROPERTY(EditAnywhere, Category="Common", BlueprintReadOnly)
	TArray<FString> ExcludeDirectories;
	
	// Modules determine what parts of style guide will be validated and enforced.
	// Modules will add up array properties and override single values,
	// starting from root directory style guide setup towards deeper nested directories.
	UPROPERTY(EditDefaultsOnly, Instanced, Category="Modules", meta=(ShowInnerProperties, EditFixedOrder))
	TArray<TObjectPtr<USGModule>> Modules;
	
};
