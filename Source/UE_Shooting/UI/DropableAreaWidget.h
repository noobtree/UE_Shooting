// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetAcceptable.h"
#include "DropableAreaWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UDropableAreaWidget : public UUserWidget, public IWidgetAcceptable
{
	GENERATED_BODY()

protected:
	// UMG에서의 BeginPlay 역할을 하는 함수(Construct)에 대한 C++ 초기화 함수
	virtual void NativeConstruct() override;

	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	// 드래그한 UI가 드롭될 수 있는 영역을 지정하는 Canvas Panel
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<class UCanvasPanel> dropableAreaCanvasPanel;

public:
	virtual UPanelSlot* AddChildWidget_Implementation(UUserWidget* widget) override;

};
