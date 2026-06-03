// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "UE_Shooting/Managers/LocalWidgetManager.h"

void AShootingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalPlayerController() == false)
	{
		return;
	}

	// InputMappingContext 등록
	if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		subSystem->AddMappingContext(mappingContext, 0);
	}

	// 기본 HUD UI 생성
	ULocalPlayer* localPlayer = GetLocalPlayer();
	ULocalWidgetManager* widgetManager = localPlayer->GetSubsystem<ULocalWidgetManager>();
	if (widgetManager != nullptr)
	{
		widgetManager->FindOrAddWidget(hudWidgetClass);
	}
}

bool AShootingPlayerController::AddWidgetToViewport(TSubclassOf<UUserWidget> widgetClass, UUserWidget*& widgetInstance)
{
	if (IsValid(widgetInstance) == false)
	{
		widgetInstance = CreateWidget<UUserWidget>(this, widgetClass);
	}

	if (widgetInstance->IsInViewport() == false)
	{
		widgetInstance->AddToViewport();
	}

	return IsValid(widgetInstance);
}
