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
	void AddWeaponClass(TSubclassOf<AWeaponBase> weaponClass);

	// 임의 무기를 장착(부착)하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void AttachWeaponMeshes(AWeaponBase* weaponActor, USkeletalMeshComponent* firstPersonWeaponMesh, USkeletalMeshComponent* thirdPersonWeaponMesh);

	// 무기를 이용한 공격에 대하여 Montage를 재생하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void PlayWeaponMontage(UAnimMontage* firstPersonMontage, UAnimMontage* thirdPersonMontage);

	// 장비하고있는 무기의 상황을 UI에 업데이트하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void UpdateWeaponHUDWidget(AWeaponBase* weaponActor);

	// 장비하고있는 무기를 이용하여 공격되는 지점(좌표)를 반환하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	FVector GetWeaponTargetLocation();

	// 기존에 보유하고 있으면서 비활성화 되어있는 무기를 활성화하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void ActivateWeapon(AWeaponBase* weaponActor);	// 무기 교체에 사용
	
	// 기존에 보유하고 있으면서 활성화 되어있는 무기를 비활성화하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void DeativateWeapon(AWeaponBase* weaponActor);	// 무기 교체에 사용
};
