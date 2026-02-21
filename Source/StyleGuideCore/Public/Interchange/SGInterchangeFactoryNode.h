// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Nodes/InterchangeFactoryBaseNode.h"
#include "SGInterchangeFactoryNode.generated.h"

/**
 * 
 */
UCLASS()
class STYLEGUIDECORE_API USGInterchangeFactoryNode : public UInterchangeFactoryBaseNode
{
	GENERATED_BODY()

public:
	virtual UClass* GetObjectClass() const override;
};
