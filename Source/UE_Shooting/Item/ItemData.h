// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class UE_SHOOTING_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()


public:
    // AssetManager에서 생성된 DataAsset을 인식하기 위하여 함수 오버라이딩
    FORCEINLINE virtual FPrimaryAssetId GetPrimaryAssetId() const override { return FPrimaryAssetId(itemType, GetFName()); }

public:
    // 아이템 데이터 타입
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
    FPrimaryAssetType itemType;

    // 아이템 모델링 Mesh
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
    TSoftObjectPtr<USkeletalMesh> itemMesh;

    // 아이템 아이콘 Texture
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ItemData")
    TSoftObjectPtr<UTexture> itemTexture;
};
