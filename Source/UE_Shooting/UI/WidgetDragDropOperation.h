// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/DragDropOperation.h"
#include "WidgetDragDropOperation.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UWidgetDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:
	// 화면에서 드래그 & 드롭하고자 하는 UI를 저장하는데 사용되는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	UUserWidget* widgetReference;

	// UI 드래그 시작 시점에 해당 UI의 Pivot 기준 마우스를 누른 지점의 좌표를 저장하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FVector2D dragOffset;

	// 드래그 & 드롭하고자 하는 UI의 사이즈 (Width, Height)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FVector2D widgetSize;

	// 드래그하는 UI가 붙어있던 CanvasPanel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	class UCanvasPanel* sourceCanvasPanel;

	// 드래그하는 UI의 기존 Anchors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FAnchors sourceAnchors;
	
	// 드래그하는 UI의 기존 Alignments
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FVector2D sourceAlignments;

	// 드래그하는 UI의 기존 위치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	FVector2f sourcePosition;

	virtual void DragCancelled_Implementation(const FPointerEvent& PointerEvent) override;
};
