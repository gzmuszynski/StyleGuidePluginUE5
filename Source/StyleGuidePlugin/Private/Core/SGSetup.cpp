// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/SGSetup.h"

void USGSetup::ImportStyleGuide()
{
	
}

void USGSetup::ExportStyleGuide()
{
	
}

void USGSetup::MergeSettings(USGSetup* Setup)
{
	if (Setup->Settings.Override_AllowedCharacters)
	{
		Settings.Override_AllowedCharacters = true;
		Settings.AllowedCharacters = Setup->Settings.AllowedCharacters;
	}
	for (FString & ExcludedDir : Setup->Settings.ExcludeDirectories)
	{
		FString ExcludedDirFullPath = FPaths::GetPath(Setup->GetPathName()) / ExcludedDir;
		Settings.ExcludeDirectories.Add(ExcludedDirFullPath);
	}
}
