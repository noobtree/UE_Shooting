// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingHUD.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UShootingHUD::NativeConstruct()
{
}

UPanelSlot* UShootingHUD::AddContent(UWidget* widget)
{
	if (IsValid(rootCanvasPanel) == true)
	{
		return rootCanvasPanel->AddChildToCanvas(widget);
	}
	return nullptr;
}
