// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/SGModule.h"
#include "Core/SGValidationEnums.h"
#include "SGReferenceSizeModule.generated.h"

/**
 * 
 */

class IAssetManagerEditorModule;
class IAssetRegistry;

USTRUCT()
struct STYLEGUIDEMODULES_API FSGReferenceSizeNode
{
	GENERATED_BODY()
	
	FAssetIdentifier AssetIdentifier;
	FAssetIdentifier OriginalAssetIdentifier;
	const IAssetRegistry* AssetRegistry;
	IAssetManagerEditorModule* EditorModule;
	bool bIsGameResource;
};

USTRUCT()
struct STYLEGUIDEMODULES_API FSGReferenceSizeResponse
{
	GENERATED_BODY()
	
	TArray<FName> DependenciesVisited;
	TArray<FName> ReferencersVisited;
	
	uint64 EditorDiskSize = 0;
	uint64 GameDiskSize = 0;
	uint64 EditorMemorySize = 0;
	uint64 GameMemorySize = 0;
	
	int CircularDependencies = 0; 
};

UCLASS(DisplayName="Reference Count and Asset Size", BlueprintType)
class STYLEGUIDEMODULES_API USGReferenceSizeModule : public USGModule
{
	GENERATED_BODY()

public:
	virtual bool CanValidateAsset(const FAssetData& AssetData, UObject* Object,
		FDataValidationContext& Context) const override;
	virtual EDataValidationResult ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
		const FDataValidationContext& Context) override;
	virtual void MergeModuleSettings_Implementation(USGModule* Module) override;
	
protected:
	
	UFUNCTION(BlueprintCallable, Category="Reference Size", meta=(BlueprintProtected))
	bool GatherDependenciesData(const FName& AssetIdentifier, int64 &EditorDiskSize, int64 &GameDiskSize, int64 &EditorMemorySize, int64 &GameMemorySize, int &Dependencies, int &Referencers, int
	                            & CircularDependencies);

	static bool VisitDependencyNode(const FSGReferenceSizeNode& Node, FSGReferenceSizeResponse& Response);
	static bool VisitReferencerNode(const FSGReferenceSizeNode& Node, FSGReferenceSizeResponse& Response);
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity EditorDiskSizeVerbosity = ESGValidationVerbosity::Hint;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly, 
		meta=(EditConditionHides, EditCondition="EditorDiskSizeVerbosity != ESGValidationVerbosity::None"))
	int64 AllowedEditorDiskSize = 134217728;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity GameDiskSizeVerbosity = ESGValidationVerbosity::Warning;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly, 
	meta=(EditConditionHides, EditCondition="GameDiskSizeVerbosity != ESGValidationVerbosity::None"))
	int64 AllowedGameDiskSize = 134217728;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity EditorMemorySizeVerbosity = ESGValidationVerbosity::Hint;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly, 
	meta=(EditConditionHides, EditCondition="EditorMemorySizeVerbosity != ESGValidationVerbosity::None"))
	int64 AllowedEditorMemorySize = 134217728;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity GameMemorySizeVerbosity = ESGValidationVerbosity::Warning;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly, 
	meta=(EditConditionHides, EditCondition="GameMemorySizeVerbosity != ESGValidationVerbosity::None"))
	int64 AllowedGameMemorySize = 134217728;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity DependenciesVerbosity = ESGValidationVerbosity::Warning;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly,
		meta=(EditConditionHides, EditCondition="DependenciesVerbosity != ESGValidationVerbosity::None"))
	uint8 DependenciesLimit = 20;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity ReferencersVerbosity = ESGValidationVerbosity::Warning;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly,
		meta=(EditConditionHides, EditCondition="ReferencersVerbosity != ESGValidationVerbosity::None"))
	uint8 ReferencersLimit = 20;
	
	UPROPERTY(EditAnywhere, Category="Reference and Size", BlueprintReadOnly)
	ESGValidationVerbosity CircularDependenciesVerbosity = ESGValidationVerbosity::Warning;
};
