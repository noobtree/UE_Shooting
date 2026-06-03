// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShootingHUD.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class UE_SHOOTING_API UShootingHUD : public UUserWidget
{
	GENERATED_BODY()

public:
    // UMG 위젯이 화면에 “실제로 생성되고 붙을 때” 호출되는 C++ 초기화 함수
    virtual void NativeConstruct() override;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"))
    TObjectPtr<class UCanvasPanel> rootCanvasPanel;

public:
    UFUNCTION(BlueprintCallable)
    UPanelSlot* AddContent(UWidget* widget);
};
