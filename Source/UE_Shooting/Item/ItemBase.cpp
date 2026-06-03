// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemBase.h"
#include "Engine/AssetManager.h"
#include "Engine/StreamableManager.h"
#include "ItemData.h"

// Sets default values
AItemBase::AItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	defaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRootComponent"));
	SetRootComponent(defaultSceneRoot);
	
	// 1인칭 Mesh Component 설정
	firstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh1P"));
	firstPersonMeshComponent->SetupAttachment(RootComponent);
	firstPersonMeshComponent->SetOnlyOwnerSee(true);
	firstPersonMeshComponent->SetOwnerNoSee(false);

	// 3인칭 MeshComponent 설정
	thirdPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("Mesh3P"));
	thirdPersonMeshComponent->SetupAttachment(RootComponent);
	thirdPersonMeshComponent->SetOnlyOwnerSee(false);
	thirdPersonMeshComponent->SetOwnerNoSee(true);
}

// Called when the game starts or when spawned
void AItemBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemBase::AssignItemData(FPrimaryAssetId itemDataID)
{
	// 아이템 데이터 아이디 유효성 확인
	if (itemDataID.IsValid() == false)
	{
		return;
	}

	UAssetManager& assetManager = UAssetManager::Get();
	// DataAsset의 ID를 통하여 데이터 비동기 로드
	TArray<FName> Bundles;
	FStreamableDelegate OnCompletedAsyncLoad = FStreamableDelegate::CreateUObject(this, &AItemBase::OnCompletedAsyncLoadPrimaryAsset, itemDataID);

	asyncLoadHandle = assetManager.LoadPrimaryAsset(itemDataID, Bundles, OnCompletedAsyncLoad);
	return;
}

void AItemBase::LeftMouseButtonTap()
{
	UE_LOG(LogTemp, Display, TEXT("Item(%s) - Left Mouse Button Tap"), *GetName());
}

void AItemBase::LeftMouseButtonHold()
{
	UE_LOG(LogTemp, Display, TEXT("Item(%s) - Left Mouse Button Hold"), *GetName());
}

void AItemBase::RightMouseButtonTap()
{
	UE_LOG(LogTemp, Display, TEXT("Item(%s) - Right Mouse Button Tap"), *GetName());
}

void AItemBase::RightMouseButtonHold()
{
	UE_LOG(LogTemp, Display, TEXT("Item(%s) - Right Mouse Button Hold"), *GetName());
}

void AItemBase::OnCompletedAsyncLoadPrimaryAsset_Implementation(FPrimaryAssetId itemDataID)
{
	UItemData* itemData = UAssetManager::Get().GetPrimaryAssetObject<UItemData>(itemDataID);
	if (itemData == nullptr)
	{
		return;
	}

	FStreamableManager& streamableManager = UAssetManager::GetStreamableManager();

	// DataAsset 내부 Soft Reference를 비동기 로드하여 적용
	FStreamableDelegate asyncStreamableDelegator = FStreamableDelegate::CreateLambda([&]() { 
		firstPersonMeshComponent->SetSkeletalMesh(itemData->itemMesh.Get()); 
		thirdPersonMeshComponent->SetSkeletalMesh(itemData->itemMesh.Get());
		});
	streamableManager.RequestAsyncLoad(itemData->itemMesh.ToSoftObjectPath(), asyncStreamableDelegator);
}
