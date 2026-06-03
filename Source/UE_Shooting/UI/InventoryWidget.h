// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DragableWidget.h"
#include "InventoryWidget.generated.h"

class UInventoryComponent;

USTRUCT(BlueprintType)
struct UE_SHOOTING_API FEdge2D
{
	GENERATED_BODY()

public:
	/** Default constructor (no initialization). */
	FEdge2D() {}

	/**
	 * Creates and initializes a new edge from two vertices.
	 *
	 * @param V1 The first vertex.
	 * @param V2 The second vertex.
	 */
	FEdge2D(FVector2D V1, FVector2D V2)
	{
		Vertex[0] = V1;
		Vertex[1] = V2;
	}

public:
	/** Holds the edge vertices. */
	FVector2D Vertex[2];

public:
	/**
	 * Compares this edge with another.
	 *
	 * @param E The other edge.
	 * @return true if the two edges are identical, false otherwise.
	 */
	bool operator== (const FEdge2D& E) const
	{
		return (((E.Vertex[0] == Vertex[0]) && (E.Vertex[1] == Vertex[1])) || ((E.Vertex[0] == Vertex[1]) && (E.Vertex[1] == Vertex[0])));
	}
};

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UInventoryWidget : public UDragableWidget
{
	GENERATED_BODY()

protected:
	// UMG 위젯이 화면에 “실제로 생성되고 붙을 때” 호출되는 C++ 초기화 함수
	virtual void NativeConstruct() override;

protected:
	// 인벤토리의 UI의 크기를 조절하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class USizeBox> sizeController;

	// 인벤토리의 각 Cell을 보관, 관리하는 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UUniformGridPanel> cellGridPanel;

	// UI에 표시되는 인벤토리 객체
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* inventoryComponent;

	// 인벤토리 1칸의 크기 (Width == Height)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float cellSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEdge2D> arrWidgetEdge;

public:
	// 임의 인벤토리와 Cell 크기를 사용하여 UI를 초기화하는 함수
	UFUNCTION(BlueprintCallable)
	virtual void InitializeWidget(UInventoryComponent* Inventory, float SquareSize);

protected:
	UFUNCTION(BlueprintCallable)
	virtual void InitializeWidgetEdgeSegments();

	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
};
