// Fill out your copyright notice in the Description page of Project Settings.


#include "ReferenceSize/SGReferenceSizeModule.h"

#include "AssetManagerEditorModule.h"
#include "AssetRegistry/AssetRegistryModule.h"

#define LOCTEXT_NAMESPACE "StyleGuideReferenceSize"

#define CheckValuesUnit(Description, Value, AllowedValue, Verbosity, Unit) \
if (Verbosity != ESGValidationVerbosity::None && Value > AllowedValue)\
{\
	FFormatNamedArguments Args;\
	Args.Add(TEXT("Value"), FText::FromString(FString::FromInt(Value)));\
	Args.Add(TEXT("AllowedValue"), FText::FromString(FString::FromInt(AllowedValue)));\
	Args.Add(TEXT("Unit"), FText::FromString(##Unit));\
	EDataValidationResult TempResult = SubmitValidationFailEvent( Verbosity, InAsset, \
	FText::Format(LOCTEXT(#Description "Failed", \
	"Asset exceeds maximum allowed " #Description " of {AllowedValue}{Unit} ({Value}{Unit})"), Args));\
	if (Result < TempResult)\
	{\
	Result = TempResult;\
	}\
}

#define CheckValues(Description, Value, AllowedValue, Verbosity) CheckValuesUnit(Description, Value, \
AllowedValue, Verbosity, "")

#define CheckSize(Configuration, Type) CheckValuesUnit(##Configuration##Type##Size, ##Configuration##Type##Size, \
	Allowed##Configuration##Type##Size, ##Configuration##Type##SizeVerbosity, "B")

bool USGReferenceSizeModule::CanValidateAsset(const FAssetData& AssetData, UObject* Object,
                                              FDataValidationContext& Context) const
{
	return Object->HasAllFlags(RF_Standalone) && Super::CanValidateAsset(AssetData, Object, Context);
}

EDataValidationResult USGReferenceSizeModule::ValidateLoadedAsset(const FAssetData& InAssetData, UObject* InAsset,
	const FDataValidationContext& Context)
{
	int64 EditorDiskSize, GameDiskSize, EditorMemorySize, GameMemorySize;
	int Dependencies, Referencers, CircularDependencies;
	
	GatherDependenciesData(InAssetData.PackageName, EditorDiskSize, GameDiskSize, 
		EditorMemorySize, GameMemorySize, Dependencies, Referencers, CircularDependencies);
	
	//I know what you probably think of macros in context of debugging and ease of read, 
	//but they make repetitive blocks easier to write, so I use them.
	//Especially so, that my IDE of choice (Rider) can display macros expansion previews.
	
	CheckSize(Editor, Disk);
	CheckSize(Game, Disk);
	CheckSize(Editor, Memory);
	CheckSize(Game, Memory);
	
	CheckValues(Dependencies, Dependencies, DependenciesLimit, DependenciesVerbosity);
	CheckValues(Referencers, Referencers, ReferencersLimit, ReferencersVerbosity);
	CheckValues(CircularDependencies, CircularDependencies, 0, CircularDependenciesVerbosity);
	
	return Result;
}

void USGReferenceSizeModule::MergeModuleSettings_Implementation(USGModule* Module)
{
	if (const auto ReferenceSizeModule = Cast<USGReferenceSizeModule>(Module))
	{
		EditorDiskSizeVerbosity = ReferenceSizeModule->EditorDiskSizeVerbosity;
		AllowedEditorDiskSize = ReferenceSizeModule->AllowedEditorDiskSize;
		GameDiskSizeVerbosity = ReferenceSizeModule->GameDiskSizeVerbosity;
		AllowedGameDiskSize = ReferenceSizeModule->AllowedGameDiskSize;
		EditorMemorySizeVerbosity = ReferenceSizeModule->EditorMemorySizeVerbosity;
		AllowedEditorMemorySize = ReferenceSizeModule->AllowedEditorMemorySize;
		GameMemorySizeVerbosity = ReferenceSizeModule->GameMemorySizeVerbosity;
		AllowedGameMemorySize = ReferenceSizeModule->AllowedGameMemorySize;
		DependenciesVerbosity = ReferenceSizeModule->DependenciesVerbosity;
		DependenciesLimit = ReferenceSizeModule->DependenciesLimit;
		ReferencersVerbosity = ReferenceSizeModule->ReferencersVerbosity;
		ReferencersLimit = ReferenceSizeModule->ReferencersLimit;
		CircularDependenciesVerbosity = ReferenceSizeModule->CircularDependenciesVerbosity;
	}
	Super::MergeModuleSettings_Implementation(Module);
}

bool USGReferenceSizeModule::GatherDependenciesData(const FName& AssetIdentifier, int64& EditorDiskSize,
                                                    int64& GameDiskSize, int64& EditorMemorySize, int64& GameMemorySize, int& Dependencies, int& Referencers, int&
                                                    CircularDependencies)
{
	FSGReferenceSizeResponse Response;
	const IAssetRegistry& Registry = FModuleManager::GetModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	IAssetManagerEditorModule& ManagerEditorModule = IAssetManagerEditorModule::Get();
	ManagerEditorModule.GetCurrentRegistrySource(false);
	
	const FSGReferenceSizeNode Node = { AssetIdentifier, AssetIdentifier, &Registry, &ManagerEditorModule };
	VisitDependencyNode(Node, Response);
	VisitReferencerNode(Node, Response);
	
	EditorDiskSize = Response.EditorDiskSize;
	GameDiskSize = Response.GameDiskSize;
	EditorMemorySize = Response.EditorMemorySize;
	GameMemorySize = Response.GameMemorySize;
	Dependencies = Response.DependenciesVisited.Num()-1;
	Referencers = Response.ReferencersVisited.Num()-1;
	CircularDependencies = Response.CircularDependencies;

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
		if (Dependency.ToString().StartsWith(TEXT("/Script/")))
		{
			continue;
		}
		if (Dependency == Node.OriginalAssetIdentifier)
		{
			Response.CircularDependencies++;
		}
		if (!Response.DependenciesVisited.Contains(Dependency) && !AssetPackageNameString.StartsWith(TEXT("/Script/")))
		{
			FSGReferenceSizeNode DependencyNode = { Dependency , Node.OriginalAssetIdentifier,
				Node.AssetRegistry, Node.EditorModule, GameDependencies.Contains(Dependency)};
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
		// If you have a /Script/ class referencing your assets, you fucked up big time.
		if (Referencer.ToString().StartsWith(TEXT("/Script/")))
		{
			continue;
		}
		if (!Response.ReferencersVisited.Contains(Referencer) && !AssetPackageNameString.StartsWith(TEXT("/Script/")))
		{
			FSGReferenceSizeNode DependencyNode = { Referencer, Node.OriginalAssetIdentifier,
				Node.AssetRegistry, Node.EditorModule};
			VisitReferencerNode(DependencyNode, Response);
		}
	}
	return !Referencers.IsEmpty();
}

#undef LOCTEXT_NAMESPACE
#undef CheckSize
#undef CheckValues
#undef CheckValuesUnit