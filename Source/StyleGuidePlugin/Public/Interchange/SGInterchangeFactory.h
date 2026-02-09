// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InterchangeFactoryBase.h"
#include "SGInterchangeFactory.generated.h"

/**
 * 
 */
UCLASS()
class STYLEGUIDEPLUGIN_API USGInterchangeFactory : public UInterchangeFactoryBase
{
	GENERATED_BODY()

public:
	virtual UClass* GetFactoryClass() const override;
	virtual FImportAssetResult BeginImportAsset_GameThread(const FImportAssetObjectParams& Arguments) override;
};
