// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputBindable.h"
#include "HandComponent.generated.h"

class AItemBase;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_SHOOTING_API UHandComponent : public UActorComponent, public IInputBindable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHandComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// IInputBindable을(를) 통해 상속됨
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// 손에 들고있는 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	AItemBase* actorInHand;

public:
	// 아이템을 손에 들기 (교체)
	void TakeItem(FPrimaryAssetId itemDataID);
	//// 아이템을 손에서 놓기
	//void LayItem();

	//// 좌클릭
	//void OnAttackAction();
	//// 우클릭
	//void OnUseAction();'
};
