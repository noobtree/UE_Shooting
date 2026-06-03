// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ItemBase.h"
#include "ItemTableRow.generated.h"

/**
 * 
 */
USTRUCT()
struct UE_SHOOTING_API FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()
	
public:
	FItemTableRow();

public:
	// 아이템 이름
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName itemName;

	// 아이템 드롭률
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float itemDropRate;

	// 아이템 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AItemBase> itemClass;

	// 아이템 데이터 아이디
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FPrimaryAssetId itemDataID;
};
