// Fill out your copyright notice in the Description page of Project Settings.


#include "Interchange/SGInterchangeFactoryNode.h"

#include "Core/SGSetup.h"

UClass* USGInterchangeFactoryNode::GetObjectClass() const
{
	return USGSetup::StaticClass();
}
