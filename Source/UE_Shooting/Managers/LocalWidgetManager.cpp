// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalWidgetManager.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

ULocalWidgetManager* ULocalWidgetManager::GetLocalWidgetManager(const UObject* WorldContextObject)
{
	if (!WorldContextObject)
	{
		return nullptr;
	}

	APlayerController* controller = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	if (controller == nullptr)
	{
		return nullptr;
	}

	ULocalPlayer* localPlayer = controller->GetLocalPlayer();
	if (localPlayer == nullptr)
	{
		return nullptr;
	}

	return ULocalPlayer::GetSubsystem<ULocalWidgetManager>(localPlayer);
}

UUserWidget* ULocalWidgetManager::FindOrAddWidget(const TSubclassOf<UUserWidget>& widgetClass, int32 ZOrder)
{
	// 찾으려는 Widget의 Class 유효성 확인
	if (IsValid(widgetClass) == false)
	{
		return nullptr;
	}

	// 로컬 플레이어 객체 얻기
	ULocalPlayer* localPlayer = GetLocalPlayer();
	if (IsValid(localPlayer) == false)
	{
		return nullptr;
	}

	// 키에 해당하는 Widget 정보가 존재하는지 확인
	UUserWidget*&& widgetInstance = widgetMap.FindOrAdd(widgetClass);
	if (IsValid(widgetInstance) == false)
	{
		// Widget 인스턴스 생성
		widgetInstance = CreateWidget<UUserWidget>(localPlayer->GetPlayerController(GetWorld()), widgetClass);

		// 생성된 인스턴스 유효성 검사
		if (IsValid(widgetInstance) == false)
		{
			// 유효하지 않은 키 삭제
			widgetMap.Remove(widgetClass);
			return nullptr;
		}

		// Viewport에 Widget 등록
		widgetInstance->AddToPlayerScreen(ZOrder);
	}

	// Widget의 인스턴스 반환
	return widgetInstance;
}

bool ULocalWidgetManager::AddWidget(const TSubclassOf<UUserWidget>& widgetClass, UUserWidget* widgetInstance)
{
	// 등록하려는 Widget의 Class 유효성 확인
	if (IsValid(widgetClass) == true && widgetInstance != nullptr)
	{
		// 등록하려는 인스턴스의 클래스 검사
		if (widgetInstance->IsA(widgetClass) == false)
		{
			return false;
		}

		// 기존에 등록되어있는 인스턴스가 존재하는지 확인
		if (widgetMap.Contains(widgetClass) == false)
		{
			// Map에 Widget 등록
			widgetMap.Add(widgetClass, widgetInstance);
			return true;
		}

		// 기존에 등록되어있는 인스턴스 유효성 검사
		UUserWidget* instance = widgetMap[widgetClass];
		if (IsValid(instance) == false)
		{
			// 등록된 인스턴스 교체
			widgetMap[widgetClass] = widgetInstance;
			return true;
		}

		// 기존에 등록된 인스턴스와 새로 등록하려는 인스턴스의 동일성 검사
		return instance == widgetInstance;
	}

	return false;
}

bool ULocalWidgetManager::RemoveWidget(const TSubclassOf<UUserWidget>& widgetClass)
{
	// 제거하려는 Widget의 Class 유효성 확인
	if (IsValid(widgetClass) == true)
	{
		// Map에서 Key 값 제거 및 해당 클래스로 등록된 인스턴스 얻기
		UUserWidget* widgetInstance = widgetMap.FindAndRemoveChecked(widgetClass);

		// 생성되어있는 인스턴스의 유효성 확인
		if (IsValid(widgetInstance) == true)
		{
			// 화면에서 Widget 제거
			widgetInstance->RemoveFromParent();
		}
		return true;
	}
	return false;
}

void ULocalWidgetManager::SetWidgetHiddenInGame(const TSubclassOf<UUserWidget>& widgetClass, bool bNewHidden)
{
	// 변경하려는 Widget의 Class 유효성 확인
	if (IsValid(widgetClass) == true && widgetMap.Contains(widgetClass) == true)
	{
		// 인스턴스 불러오기
		TObjectPtr<UUserWidget>& widgetInstance = widgetMap[widgetClass];

		// 인스턴스 유효성 검사
		if (IsValid(widgetInstance) == true)
		{
			// Visibility 속성 변경
			widgetInstance->SetVisibility(bNewHidden == true ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		}
	}
	return;
}

void ULocalWidgetManager::ClearWidgetInGame()
{
	// WidgetMap에 등록된 모든 Pair 조회
	for (auto pair : widgetMap)
	{
		// 생성된 Widget 인스턴스의 유효성 검사
		if (IsValid(pair.Value) == true)
		{
			// 화면에서 Widget 제거
			pair.Value->RemoveFromParent();
		}
	}

	// Key-Value 목록 초기화
	widgetMap.Empty();
	return;
}
