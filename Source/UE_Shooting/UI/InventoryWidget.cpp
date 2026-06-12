// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/SizeBox.h"
#include "Components/UniformGridPanel.h"
#include "Components/InventoryComponent.h"

void UInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInventoryWidget::InitializeWidget(UInventoryComponent* Inventory, float SquareSize)
{
	UE_LOG(LogTemp, Display, TEXT("Initialize Inventory Widget"));

	inventoryComponent = Inventory;
	cellSize = SquareSize;

	if (IsValid(sizeController) == false || IsValid(cellGridPanel) == false)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Invalid Widget Tree"));
		return;
	}

	// Inventory UI의 Cell 배치 개수 얻기 (Column, Row)
	FVector2D cellCount = inventoryComponent->GetInventoryCellCount();

	// 인벤토리 UI의 크기 설정
	sizeController->SetWidthOverride(cellSize * cellCount.X);
	sizeController->SetHeightOverride(cellSize * cellCount.Y);

	// UI Widget의 구분선 초기화
	InitializeWidgetEdgeSegments();
}

void UInventoryWidget::InitializeWidgetEdgeSegments()
{
	// UI의 Edge 목록 초기화
	arrWidgetEdge.Empty(0);

	FVector2D cellCount = inventoryComponent == nullptr ? FVector2D::ZeroVector : inventoryComponent->GetInventoryCellCount();

	// Column 구분 Edge 추가 (Vertical Edge)
	for (int32 column = 0; column <= cellCount.X; ++column)
	{
		FVector2D vertex1 = FVector2D(cellSize * column, 0);
		FVector2D vertex2 = FVector2D(cellSize * column, cellSize * cellCount.Y);

		FEdge2D widgetEdge = FEdge2D(vertex1, vertex2);
		arrWidgetEdge.Add(widgetEdge);
	}

	// Row 구분 Edge 추가 (Horizontal Edge)
	for (int32 row = 0; row <= cellCount.Y; ++row)
	{
		FVector2D vertex1 = FVector2D(0, cellSize * row);
		FVector2D vertex2 = FVector2D(cellSize * cellCount.X, cellSize * row);

		FEdge2D widgetEdge = FEdge2D(vertex1, vertex2);
		arrWidgetEdge.Add(widgetEdge);
	}
}

int32 UInventoryWidget::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, 
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	const int32 superLayerId = Super::NativePaint(Args, AllottedGeometry, MyCullingRect, OutDrawElements, LayerId, InWidgetStyle, bParentEnabled);

	// UMG DrawLine 노드와 같은 방식
	FPaintContext drawContext = FPaintContext(AllottedGeometry, MyCullingRect, OutDrawElements, superLayerId + 1, InWidgetStyle, bParentEnabled);

	FVector2D lefttop = FVector2D(AllottedGeometry.Position);

	// Widget Edge 그리기
	for (const FEdge2D& widgetEdge : arrWidgetEdge)
	{
		FVector2D vertex1 = lefttop + widgetEdge.Vertex[0];
		FVector2D vertex2 = lefttop + widgetEdge.Vertex[1];
		// Widget의 Local LeftTop 좌표는 항상 (0, 0)으로 고정
		UWidgetBlueprintLibrary::DrawLine(drawContext, vertex1, vertex2, FLinearColor::Red);
		//TArray<FVector2D> vertices = TArray<FVector2D>(widgetEdge.Vertex, 2);
		//FSlateDrawElement::MakeLines(OutDrawElements, superLayerId + 1, AllottedGeometry.ToPaintGeometry(), vertices, ESlateDrawEffect::None, FLinearColor::Black, true, 1.f);
	}

	return superLayerId + 1;
}
