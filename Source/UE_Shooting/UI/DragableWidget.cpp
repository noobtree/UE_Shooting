// Fill out your copyright notice in the Description page of Project Settings.


#include "DragableWidget.h"
#include "Components/Border.h"
#include "Components/SizeBox.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/VerticalBox.h"
#include "WidgetDragDropOperation.h"

void UDragableWidget::NativeConstruct()
{
	Super::NativeConstruct();

	bIsDragDropable = true;
	SetClipping(EWidgetClipping::ClipToBounds);
}

FReply UDragableWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (IsValid(cachedCanvasSlot) == false)
	{
		cachedCanvasSlot = Cast<UCanvasPanelSlot>(Slot);
		if (IsValid(cachedCanvasSlot) == false)
		{
			return FReply::Unhandled();
		}
	}

	FReply reply = Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	// 드래그 가능한 Widget의 유효성 확인
	if (IsValid(dragableBorder) == false)
	{
		return reply;
	}
	
	// 마우스 위치 좌표 얻기
	FVector2D screenMousePosition = InMouseEvent.GetScreenSpacePosition();

	// 마우스 버튼을 누른 지점이 Border의 영역 내부에 존재하는지 확인
	if (dragableBorder->GetCachedGeometry().IsUnderLocation(screenMousePosition) == false)
	{
		return reply;
	}

	// 마우스 좌클릭이 활성화된 상태에서 Drag 이벤트 감지
	return FReply::Handled().DetectDrag(this->GetCachedWidget().ToSharedRef(), EKeys::LeftMouseButton);
}

FReply UDragableWidget::NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	return RedirectMouseDownToWidget(InGeometry, InMouseEvent).NativeReply;
}

void UDragableWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	if (bIsDragDropable == false)
	{
		return;
	}

	SetVisibility(ESlateVisibility::HitTestInvisible);

	UWidgetDragDropOperation* dragDropOperation = NewObject<UWidgetDragDropOperation>();
	// 드래그를 위한 정보 저장
	dragDropOperation->widgetReference = this;		// 드래그 & 드롭되는 Widget 저장
	dragDropOperation->dragOffset = InGeometry.AbsoluteToLocal(InMouseEvent.GetScreenSpacePosition());		// 드래그 키 입력 좌표와 실제 UI 좌표 차이 저장
	dragDropOperation->widgetSize = InGeometry.GetLocalSize();

	// 드래그 실패 시 UI 복구를 위한 정보 저장
	dragDropOperation->sourceCanvasPanel = Cast<UCanvasPanel>(GetParent());
	dragDropOperation->sourceAnchors = cachedCanvasSlot->GetAnchors();
	dragDropOperation->sourceAlignments = cachedCanvasSlot->GetAlignment();
	dragDropOperation->sourcePosition = InGeometry.Position;

	// 드래그 동작 중 화면에 표시될 Widget 설정
	if (IsValid(dragVisualClass) == true)
	{
		dragDropOperation->DefaultDragVisual = CreateWidget<UUserWidget>(GetOwningPlayer(), dragVisualClass);
	}
	else
	{
		dragDropOperation->DefaultDragVisual = this;
		RemoveFromParent();
	}

	// DefaultDragVisual이 출력되는 기준점 설정
	dragDropOperation->Pivot = EDragPivot::MouseDown;

	OutOperation = dragDropOperation;
}

FEventReply UDragableWidget::RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	FEventReply reply;
	reply.NativeReply = NativeOnMouseButtonDown(InGeometry, InMouseEvent);
	return reply;
}
