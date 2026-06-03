// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "PlayerManager.generated.h"

USTRUCT(BlueprintType)
struct UE_SHOOTING_API FHealthProperty
{
	GENERATED_BODY()

public:
	// 현재 체력
	UPROPERTY()
	float currentHealth = 0;

	// 최대 체력
	UPROPERTY()
	float maxHealth = 0;
};

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UPlayerManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	// 로컬 플레이어 액터
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	TWeakObjectPtr<ACharacter> localPlayer;

	// 체력 현황이 저장되었는지를 판별하는 플래그
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool bIsHealthSaved;

	// 저장되어있는 체력 현황
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	FHealthProperty playerHealth;
};
