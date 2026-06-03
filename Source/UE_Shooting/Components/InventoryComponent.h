// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputBindable.h"
#include "InventoryComponent.generated.h"

// 인벤토리의 Grid
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FInventoryGridCellCountChangedSignature, int32, NewColumnCount, int32, NewRowCount);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_SHOOTING_API UInventoryComponent : public UActorComponent, public IInputBindable
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// IInputBindable을(를) 통해 상속됨
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	FInventoryGridCellCountChangedSignature OnGridCellCountChanged;

protected:
	// 인벤토리 UI를 열고 닫는 키의 InputAction
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<class UInputAction> IA_inventory;

	// 인벤토리 UI에서 Cell의 Column 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 columnCount;

	// 인벤토리 UI에서 Cell의 Row 개수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = "true"))
	int32 rowCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> widgetClass;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	UUserWidget* widgetInstance;

public:
	// 인벤토리 UI Grid의 { Column, Row } Cell 개수를 반환하는 함수
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FVector2D GetInventoryCellCount() const { return FVector2D(columnCount, rowCount); }

	void ToggleWidgetVisibility();
};
