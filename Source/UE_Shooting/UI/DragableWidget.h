// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DragableWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UDragableWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// UMG에서의 BeginPlay 역할을 하는 함수(Construct)에 대한 C++ 초기화 함수
	virtual void NativeConstruct() override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;

public:
	// 드래그 & 드롭 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dragable Settings")
	bool bIsDragDropable;

protected:
	// 드래그 하는 동안 마우스를 따라 화면에 노출되는 UI의 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dragable Settings|DragDropOperation")
	TSubclassOf<UUserWidget> dragVisualClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Dragable Settings")
	TObjectPtr<class UBorder> dragableBorder;

	// Widget이 부모 CanvasPanel 안에서 차지하고 있는 슬롯 정보를 저장해 둔 변수
	// WBP_DragDropableWidget이 WBP_WindowCanvas에 추가될 때 생성되는 UCanvasPanelSlot
	UPROPERTY(Transient)
	TObjectPtr<class UCanvasPanelSlot> cachedCanvasSlot;

protected:
	virtual FEventReply RedirectMouseDownToWidget(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);
};
