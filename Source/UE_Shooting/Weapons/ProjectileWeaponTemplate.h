// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectileWeaponTemplate.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UProjectileWeaponTemplate : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for weapons that shoot a projectile object of some kind
 */
class UE_SHOOTING_API IProjectileWeaponTemplate
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 발사체를 생성하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IProjectileWeaponTemplate")
	void SpawnProjectile(const int32 projectileAmount);

	// 발사될 투사체의 최초 위치, 회전, 크기 값을 무작위로 반환하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IProjectileWeaponTemplate")
	FTransform GetRandomProjectileTransform(const FVector targetLocation);

	// 무기를 재장전하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IProjectileWeaponTemplate")
	void Reload();
};
