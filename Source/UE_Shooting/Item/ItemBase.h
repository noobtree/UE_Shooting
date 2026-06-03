// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

UCLASS()
class UE_SHOOTING_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// 액터의 기준 좌표를 관리하는 SceneComponent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Base", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> defaultSceneRoot;

	// 아이템의 Mesh를 담당하는 Component (1인칭 시점)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Base", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> firstPersonMeshComponent;

	// 아이템의 Mesh를 담당하는 Component (3인칭 시점)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Base", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> thirdPersonMeshComponent;

	// DataAsset 비동기 로드를 위한 핸들
	TSharedPtr<struct FStreamableHandle> asyncLoadHandle;

public:
	UFUNCTION(BlueprintCallable)
	void AssignItemData(FPrimaryAssetId itemDataID);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetMesh1P() const { return firstPersonMeshComponent; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USkeletalMeshComponent* GetMesh3P() const { return thirdPersonMeshComponent; }

	UFUNCTION(BlueprintCallable)
	virtual void LeftMouseButtonTap();

	UFUNCTION(BlueprintCallable)
	virtual void LeftMouseButtonHold();

	UFUNCTION(BlueprintCallable)
	virtual void RightMouseButtonTap();

	UFUNCTION(BlueprintCallable)
	virtual void RightMouseButtonHold();
	
protected:
	UFUNCTION(BlueprintNativeEvent)
	void OnCompletedAsyncLoadPrimaryAsset(FPrimaryAssetId itemDataID);
	virtual void OnCompletedAsyncLoadPrimaryAsset_Implementation(FPrimaryAssetId itemDataID);
};
