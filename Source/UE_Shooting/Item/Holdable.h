// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Holdable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UHoldable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_SHOOTING_API IHoldable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 공격 키 입력(Click 또는 Tap) 시 호출하는 함수
	// Ex) Left Mouse Button
	UFUNCTION(BlueprintNativeEvent)
	void OnAttackActionTap();
	// C++ 에서 정의하는 OnAttackActionTap의 보충 함수
	virtual void OnAttackActionTap_Implementation() = 0;

	// 공격 키 입력(Hold) 시 호출하는 함수
	// Ex) Left Mouse Button
	UFUNCTION(BlueprintNativeEvent)
	void OnAttackActionHold();
	// C++ 에서 정의하는 OnAttackActionHold의 보충 함수
	virtual void OnAttackActionHold_Implementation() = 0;

	// 사용 키 입력(Click 또는 Tap) 시 호출하는 함수
	// Ex) Right Mouse Button
	UFUNCTION(BlueprintNativeEvent)
	void OnUseActionTap();
	// C++ 에서 정의하는 OnUseActionTap의 보충 함수
	virtual void OnUseActionTap_Implementation() = 0;

	// 사용 키 입력(Hold) 시 호출하는 함수
	// Ex) Right Mouse Button
	UFUNCTION(BlueprintNativeEvent)
	void OnUseActionHold();
	// C++ 에서 정의하는 OnUseActionHold의 보충 함수
	virtual void OnUseActionHold_Implementation() = 0;
};
