// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShootingPlayerController.generated.h"

class UInputMappingContext;
class UInputAction;

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API AShootingPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

public:
#pragma region Enhanced Input

	// InputMappingContext
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayPriority = "10"))
	TObjectPtr<UInputMappingContext> mappingContext;

	// Move (W, A, S, D)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayPriority = "10"))
	TObjectPtr<UInputAction> moveInputAction;

	// Jump (Space)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayPriority = "10"))
	TObjectPtr<UInputAction> jumpInputAction;

	// Look (Mouse XY Axis)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayPriority = "10"))
	TObjectPtr<UInputAction> lookInputAction;

	// Change ViewPoint (V)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayPriority = "10"))
	TObjectPtr<UInputAction> changeViewpointInputAction;

	// Attack (LMB)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (DisplayPriority = "10"))
	TObjectPtr<UInputAction> attackInputAction;

#pragma endregion

	// HUD UI 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "UI");
	TSubclassOf<UUserWidget> hudWidgetClass;
};
