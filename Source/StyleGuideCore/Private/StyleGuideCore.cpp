// Copyright Epic Games, Inc. All Rights Reserved.

#include "StyleGuideCore.h"

#include "InterchangeManager.h"
#include "Interchange/SGInterchangeFactory.h"
#include "Interchange/SGInterchangeTranslator.h"

#define LOCTEXT_NAMESPACE "FStyleGuideCoreModule"

void FStyleGuideCoreModule::StartupModule()
{
	
	UInterchangeManager& InterchangeManager = UInterchangeManager::GetInterchangeManager();
	InterchangeManager.RegisterTranslator(USGInterchangeTranslator::StaticClass());
	InterchangeManager.RegisterFactory(USGInterchangeFactory::StaticClass());
}

void FStyleGuideCoreModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FStyleGuideCoreModule, StyleGuideCore)