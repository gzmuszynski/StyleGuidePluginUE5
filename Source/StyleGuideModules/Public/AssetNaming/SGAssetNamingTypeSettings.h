// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IDetailCustomization.h"
#include "Core/SGValidationEnums.h"
#include "UObject/Object.h"
#include "SGAssetNamingTypeSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct STYLEGUIDEMODULES_API FSGAssetNamingTypeVariant
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FName Name;
	
	UPROPERTY(EditAnywhere)
	FString Prefix;
	
	UPROPERTY(EditAnywhere)
	FString Suffix;
};

USTRUCT(BlueprintType)
struct STYLEGUIDEMODULES_API FSGAssetNamingTypeSettings
{
	GENERATED_BODY()

	FSGAssetNamingTypeSettings();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowAbstract))
	TSoftClassPtr<UObject> Class;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(TitleProperty="Name"))
	TArray<FSGAssetNamingTypeVariant> Variants;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESGValidationVerbosity ValidationVerbosity;

	bool operator==(const FSGAssetNamingTypeSettings& Other) const;
	bool operator==(const TSoftClassPtr<>& Other) const;
	
	FSGAssetNamingTypeVariant& GetDefaultVariant();
};

USTRUCT(BlueprintType)
struct STYLEGUIDEMODULES_API FSGAssetNamingTypeTable
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FSGAssetNamingTypeSettings> Types;

	bool Contains(const TSoftClassPtr<UObject>& Type) const;
	FSGAssetNamingTypeSettings* Find(const TSoftClassPtr<UObject>& Type, const UObject* Object = nullptr) const;
	
	void Add(const FSGAssetNamingTypeSettings& InTypeSettings);
};
