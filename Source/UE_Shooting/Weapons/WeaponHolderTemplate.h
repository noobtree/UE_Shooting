// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "WeaponBase.h"
#include "WeaponHolderTemplate.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UWeaponHolderTemplate : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class UE_SHOOTING_API IWeaponHolderTemplate
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	// 임의 무기를 획득하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	bool AddWeaponClass(TSubclassOf<AWeaponBase> weaponClass, FInstancedStruct& deltaPropery, bool bIsSwap = false);

	// 임의 무기를 보유 목록에서 제거하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	bool RemoveWeaponClass(TSubclassOf<AWeaponBase> weaponClass);

	// 임의 무기를 장착(부착)하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void ArmWeapon(TSubclassOf<AWeaponBase> weaponClass);

	// 임의 타입의 무기를 무장 해제하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void DisarmWeapon(TSubclassOf<AWeaponBase> weaponClass);

	// 무기를 이용한 공격에 대하여 Montage를 재생하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void PlayWeaponMontage(UAnimMontage* firstPersonMontage, UAnimMontage* thirdPersonMontage);

	// 장비하고있는 무기의 상황을 UI에 업데이트하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void UpdateWeaponHUDWidget(AWeaponBase* weaponActor);

	// 장비하고있는 무기를 이용하여 공격되는 지점(좌표)를 반환하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	FVector GetWeaponTargetLocation();

};
