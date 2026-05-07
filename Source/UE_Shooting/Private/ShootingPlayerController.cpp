// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerController.h"
#include "EnhancedInputSubsystems.h"

void AShootingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController())
	{
		if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			subSystem->AddMappingContext(mappingContext, 0);
		}
		if (PlayerCameraManager != nullptr)
		{
		}
	}
}
