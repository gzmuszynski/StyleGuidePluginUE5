// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Exporters/Exporter.h"
#include "SGMarkdownExporter.generated.h"

/**
 * 
 */
UCLASS()
class STYLEGUIDECORE_API USGMarkdownExporter : public UExporter
{
	GENERATED_UCLASS_BODY()

public:
	virtual bool ExportText(const FExportObjectInnerContext* Context, UObject* Object, const TCHAR* Type,
		FOutputDevice& Ar, FFeedbackContext* Warn, uint32 PortFlags = 0) override;
};
