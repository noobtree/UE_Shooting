// Fill out your copyright notice in the Description page of Project Settings.


#include "DropableAreaWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Border.h"
#include "Components/BackgroundBlur.h"
#include "Components/CanvasPanelSlot.h"
#include "WidgetDragDropOperation.h"

void UDropableAreaWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDropableAreaWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	bool bIsOperationHandled = Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);

	UWidgetDragDropOperation* dragDropOperation = Cast<UWidgetDragDropOperation>(InOperation);
	if(IsValid(dropableAreaCanvasPanel) == false || IsValid(dragDropOperation) == false)
	{
		UE_LOG(LogTemp, Warning, TEXT("InValid Drop Operation"));
		return bIsOperationHandled;
	}

	// Root CanvasPanel의 자식으로 편입
	UCanvasPanelSlot* widgetSlot = dropableAreaCanvasPanel->AddChildToCanvas(dragDropOperation->widgetReference);
	if (IsValid(widgetSlot) == false)
	{
		return bIsOperationHandled;
	}

	const FGeometry& canvasGeometry = dropableAreaCanvasPanel->GetCachedGeometry();

	// 드랍 시점의 마우스 위치 좌표 계산
	FVector2D localMousePosition = canvasGeometry.AbsoluteToLocal(InDragDropEvent.GetScreenSpacePosition());

	// 드랍 위치에서 드래그 시점의 마우스 위치 차이 적용
	FVector2D desiredWidgetPosition = localMousePosition - dragDropOperation->dragOffset;

	// UI 위치 좌표의 한계값 계산
	FVector2D positionLimit = canvasGeometry.GetLocalSize() - dragDropOperation->widgetSize;

	// Clamping
	FVector2D newWidgetPosition = FVector2D::Clamp(desiredWidgetPosition, FVector2D::ZeroVector, positionLimit);
	
	// Widget의 Property 조정
	dragDropOperation->widgetReference->SetVisibility(ESlateVisibility::Visible);
	widgetSlot->SetAutoSize(true);
	widgetSlot->SetPosition(newWidgetPosition);

	return true;
}

UPanelSlot* UDropableAreaWidget::AddChildWidget_Implementation(UUserWidget* widget)
{
	if (IsValid(dropableAreaCanvasPanel) == true)
	{
		return dropableAreaCanvasPanel->AddChildToCanvas(widget);
	}
	return nullptr;
}
