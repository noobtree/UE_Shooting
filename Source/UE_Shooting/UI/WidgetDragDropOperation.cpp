// Fill out your copyright notice in the Description page of Project Settings.


#include "WidgetDragDropOperation.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"

void UWidgetDragDropOperation::DragCancelled_Implementation(const FPointerEvent& PointerEvent)
{
	Super::DragCancelled_Implementation(PointerEvent);
	if (IsValid(widgetReference) == false)
	{
		return;
	}

	// UI 위치 복구
	UCanvasPanelSlot* widgetSlot = sourceCanvasPanel->AddChildToCanvas(widgetReference);
	widgetReference->SetVisibility(ESlateVisibility::Visible);
	widgetSlot->SetAnchors(sourceAnchors);
	widgetSlot->SetAlignment(sourceAlignments);
	widgetSlot->SetPosition(FVector2D(sourcePosition));
	widgetSlot->SetAutoSize(true);
}
