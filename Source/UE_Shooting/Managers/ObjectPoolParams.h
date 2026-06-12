// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectPoolParams.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct UE_SHOOTING_API FObjectPoolParams
{
	GENERATED_BODY()

public:
	FObjectPoolParams()
	{
		subjectName = FName("DefaultSubject");
		subjectTemplate = nullptr;
		capacity = 1;
		bAllowOverCapacity = false;
	}

	FObjectPoolParams(const FName& subjectName, const TSubclassOf<AActor>& subjectTemplate, const int32& capacity, bool bAllowOverCapacity)
		: subjectName(subjectName), subjectTemplate(subjectTemplate), capacity(capacity), bAllowOverCapacity(bAllowOverCapacity)
	{
	}

public:
	// 서로 다른 FObjectPoolParams를 구분하는 고유 키
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName subjectName;

	// ObjectPool에서 관리되는 객체 클래스
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AActor> subjectTemplate;

	// ObjectPool에서 관리되는 객체의 기본 개수
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 capacity;

	// capacity를 초과하는 개수의 Subject를 요청받았을 경우 추가 객체 생성 가능 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAllowOverCapacity;

public:
	//FORCEINLINE bool IsValid() const { return ((subjectTemplate != nullptr) && (capacity > 0) && (subjectName.IsNone() == false)); }
};
