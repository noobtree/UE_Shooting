// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InputBindable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInputBindable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_SHOOTING_API IInputBindable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// Called to bind functionality to input at ActorComponent
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) = 0;
};
