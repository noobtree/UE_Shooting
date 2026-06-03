// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PostSetupController.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UPostSetupController : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_SHOOTING_API IPostSetupController
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual void PostSetupController(AController* Controller) = 0;
};
