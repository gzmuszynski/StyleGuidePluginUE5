// Fill out your copyright notice in the Description page of Project Settings.


#include "ReferenceSize/SGReferenceSizeModule.h"

#include "AssetManagerEditorModule.h"
#include "AssetRegistry/AssetRegistryModule.h"

bool USGReferenceSizeModule::CanValidateAsset(const FAssetData& AssetData, UObject* Object,
                                              FDataValidationContext& Context) const
{
	return Object->HasAllFlags(RF_Standalone) && Super::CanValidateAsset(AssetData, Object, Context);
}

EDataValidationResult USGReferenceSizeModule::ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
	const FDataValidationContext& Context)
{
	int64 EditorDiskSize, GameDiskSize, EditorMemorySize, GameMemorySize;
	int Dependencies, Referencers;
	
	GatherDependenciesData(InAssetData.PackageName, EditorDiskSize, GameDiskSize, EditorMemorySize, GameMemorySize, Dependencies, Referencers);
	return Super::ValidateLoadedAsset(InAssetData, InAsset, Context);
}

void USGReferenceSizeModule::MergeModuleSettings_Implementation(USGModule* Module)
{
	Super::MergeModuleSettings_Implementation(Module);
}

bool USGReferenceSizeModule::GatherDependenciesData(const FName& AssetIdentifier, int64& EditorDiskSize,
                                                    int64& GameDiskSize, int64& EditorMemorySize, int64& GameMemorySize, int& Dependencies, int& Referencers)
{
	FSGReferenceSizeResponse Response;
	const IAssetRegistry& Registry = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	IAssetManagerEditorModule& ManagerEditorModule = IAssetManagerEditorModule::Get();
	ManagerEditorModule.GetCurrentRegistrySource(false);
	
	const FSGReferenceSizeNode Node = { AssetIdentifier, &Registry, &ManagerEditorModule };
	VisitDependencyNode(Node, Response);
	VisitReferencerNode(Node, Response);
	
	EditorDiskSize = Response.EditorDiskSize;
	GameDiskSize = Response.GameDiskSize;
	EditorMemorySize = Response.EditorMemorySize;
	GameMemorySize = Response.GameMemorySize;
	Dependencies = Response.DependenciesVisited.Num()-1;
	Referencers = Response.ReferencersVisited.Num()-1;
	//TODO: Validate against target limits
	return Dependencies + Referencers > 0;
}

bool USGReferenceSizeModule::VisitDependencyNode(const FSGReferenceSizeNode& Node, FSGReferenceSizeResponse& Response)
{
	Response.DependenciesVisited.Add(Node.AssetIdentifier.PackageName);
	FAssetIdentifier AssetIdentifier = Node.AssetIdentifier;
			
	FString AssetPackageNameString = AssetIdentifier.PackageName.ToString();
	const FString AssetPathString = AssetPackageNameString + TEXT(".") + FPackageName::GetLongPackageAssetName(AssetPackageNameString);
	FAssetData AssetData = Node.AssetRegistry->GetAssetByObjectPath(AssetPathString);
	int64 FoundDiskSize = 0;
	int64 FoundResourceSize = 0;
	
	if (Node.bIsGameResource)
	{
		Node.EditorModule->GetIntegerValueForCustomColumn(AssetData, IAssetManagerEditorModule::DiskSizeName, FoundDiskSize);
	}
	Node.EditorModule->GetIntegerValueForCustomColumn(AssetData, IAssetManagerEditorModule::ResourceSizeName, FoundResourceSize);
			
	Response.EditorDiskSize += FoundDiskSize;
	Response.GameDiskSize += FoundDiskSize;
	Response.EditorMemorySize += FoundResourceSize;
	Response.GameMemorySize += FoundResourceSize;
	
	TArray<FName> Dependencies;
	TArray<FName> GameDependencies;
	
	FAssetManagerDependencyQuery Query;
	Query.Categories |= UE::AssetRegistry::EDependencyCategory::Package;
	Query.Flags |= UE::AssetRegistry::EDependencyQuery::Game;
	
	Node.AssetRegistry->GetDependencies(Node.AssetIdentifier.PackageName, Dependencies);
	Node.AssetRegistry->GetDependencies(Node.AssetIdentifier.PackageName, GameDependencies, Query.Categories, Query.Flags);
	
	for (const FName & Dependency : Dependencies)
	{
		if (!Response.DependenciesVisited.Contains(Dependency) && !AssetPackageNameString.StartsWith(TEXT("/Script/")))
		{
			FSGReferenceSizeNode DependencyNode = { Dependency, Node.AssetRegistry, Node.EditorModule, GameDependencies.Contains(Dependency)};
			VisitDependencyNode(DependencyNode, Response);
		}
	}
	return !Dependencies.IsEmpty();
}

bool USGReferenceSizeModule::VisitReferencerNode(const FSGReferenceSizeNode& Node, FSGReferenceSizeResponse& Response)
{
	Response.ReferencersVisited.Add(Node.AssetIdentifier.PackageName);
	const FAssetIdentifier AssetIdentifier = Node.AssetIdentifier;

	const FString AssetPackageNameString = AssetIdentifier.PackageName.ToString();

	TArray<FName> Referencers;
	
	Node.AssetRegistry->GetReferencers(Node.AssetIdentifier.PackageName, Referencers);
	
	for (const FName & Referencer : Referencers)
	{
		if (!Response.ReferencersVisited.Contains(Referencer) && !AssetPackageNameString.StartsWith(TEXT("/Script/")))
		{
			FSGReferenceSizeNode DependencyNode = { Referencer, Node.AssetRegistry, Node.EditorModule};
			VisitReferencerNode(DependencyNode, Response);
		}
	}
	return !Referencers.IsEmpty();
}
