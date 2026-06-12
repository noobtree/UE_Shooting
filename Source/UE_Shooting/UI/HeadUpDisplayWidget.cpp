// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadUpDisplayWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UHeadUpDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

UPanelSlot* UHeadUpDisplayWidget::AddChildWidget_Implementation(UUserWidget* widget)
{
	if (IsValid(rootCanvasPanel) == true)
	{
		return rootCanvasPanel->AddChildToCanvas(widget);
	}
	return nullptr;
}
