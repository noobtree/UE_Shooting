// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingGameMode.h"

void AShootingGameMode::StartPlay()
{
	Super::StartPlay();

	check(GEngine != nullptr);
}
