// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WeaponBase.h"
#include "WeaponHolderComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_SHOOTING_API UWeaponHolderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponHolderComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APawn* ownerPawn;

	// 보유한 무기 목록
	UPROPERTY(EditAnywhere, Category = "Weapons")
	TMap<TSubclassOf<AWeaponBase>, FInstancedStruct> ownedWeapons;

	// 현재 장착중인 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	AWeaponBase* holdWeapon;

public:

	// holderComponent

	// 임의 무기를 보유 목록에 추가하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	bool AddWeaponClass(TSubclassOf<AWeaponBase> weaponClass, FInstancedStruct& deltaPropery, bool bIsSwap = false);

	// 임의 무기를 보유 목록에서 제거하는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	bool RemoveWeaponClass(TSubclassOf<AWeaponBase> weaponClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void ArmWeapon(TSubclassOf<AWeaponBase> weaponClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "IWeaponHolderTemplate")
	void DirarmWeapon(TSubclassOf<AWeaponBase> weaponClass);

protected:
#pragma region BlueprintNativeEvent Implementation

	// 임의 무기를 보유 목록에 추가하는 함수
	virtual bool AddWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass, FInstancedStruct& deltaPropery, bool bIsSwap = false);
	// 임의 무기를 보유 목록에서 제거하는 함수
	virtual bool RemoveWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass);

	virtual void ArmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass);

	virtual void DirarmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass);

#pragma endregion

#pragma region InputAction

	//// 공격 키 입력 시 호출되는 함수
	//void AttackAction();

	//// 사용 키 입력 시 호출되는 함수
	//void UseAction();

#pragma endregion
};
