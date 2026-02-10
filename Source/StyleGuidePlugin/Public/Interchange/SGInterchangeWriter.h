// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterchangeWriterBase.h"
#include "SGInterchangeWriter.generated.h"

/**
 * 
 */
UCLASS()
class STYLEGUIDEPLUGIN_API USGInterchangeWriter : public UInterchangeWriterBase
{
	GENERATED_BODY()

public:
	virtual bool Export(UInterchangeBaseNodeContainer* BaseNodeContainer) const override;
};
