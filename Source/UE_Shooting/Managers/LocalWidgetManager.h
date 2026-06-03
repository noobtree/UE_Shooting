// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/LocalPlayerSubsystem.h"
#include "LocalWidgetManager.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API ULocalWidgetManager : public ULocalPlayerSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadWrite)
	TMap<TSubclassOf<UUserWidget>, TObjectPtr<UUserWidget>> widgetMap;

public:
	UFUNCTION(BlueprintCallable)
	static ULocalWidgetManager* GetLocalWidgetManager(const UObject* WorldContextObject);

	// 임의 클래스에 해당하는 Widget의 인스턴스를 반환하는 함수
	// 생성될 Widget의 Class가 유효하지 않으면 nullptr 반환
	// Widget의 인스턴스가 존재하지 않으면 생성 후 반환
	UFUNCTION(BlueprintCallable)
	UUserWidget* FindOrAddWidget(const TSubclassOf<UUserWidget>& widgetClass, int32 ZOrder = 0);

	// 임의 클래스에 해당하는 Widget의 Instance를 등록하는 함수
	UFUNCTION(BlueprintCallable)
	bool AddWidget(const TSubclassOf<UUserWidget>& widgetClass, UUserWidget* widgetInstance);

	// 임의 클래스에 해당하는 Widget의 Instance를 화면에서 제거하는 함수
	UFUNCTION(BlueprintCallable)
	bool RemoveWidget(const TSubclassOf<UUserWidget>& widgetClass);

	// 임의 클래스에 해당하는 Widget의 Visibility를 변경하는 함수
	UFUNCTION(BlueprintCallable)
	void SetWidgetHiddenInGame(const TSubclassOf<UUserWidget>& widgetClass, bool bNewHidden);

	// WidgetManager에서 관리중인 모든 Widget을 화면에서 제거하는 함수
	UFUNCTION(BlueprintCallable)
	void ClearWidgetInGame();
};
