// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "ShootingGameMode.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API AShootingGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;
};
