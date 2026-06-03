// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WeaponBase.h"
#include "ProjectileWeaponTemplate.h"
#include "ProjectileWeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API AProjectileWeaponBase : public AWeaponBase, public IProjectileWeaponTemplate
{
	GENERATED_BODY()
	
public:
	// 발사체
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "4_Projectile")
	TSubclassOf<AActor> projectile;

protected:
#pragma region Animations

	// 무기를 장착한 캐릭터의 1인칭 공격 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Animations")
	TObjectPtr<UAnimMontage> firstPersonReloadMontage;

	// 무기를 장착한 캐릭터의 3인칭 공격 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Animations")
	TObjectPtr<UAnimMontage> thirdPersonReloadMontage;

#pragma endregion

#pragma region Projectile

	// 총구 초속
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4_Projectile")
	float muzzleSpeed = 100;

#pragma region Ammo

	// 잔탄 수 (탄창 + 약실에 남아있는 발사체 개수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4_Projectile|Ammo")
	int32 ammoInMag;

	// 탄창의 크기 (1번 재장전으로 채워질 수 있는 발사체의 최대 개수)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4_Projectile|Ammo")
	int32 magazineSize;

	// 남은 발사체의 수량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4_Projectile|Ammo")
	int32 remainingAmmo;

#pragma endregion

#pragma region Aim

	// 명중률 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4_Projectile|Aim")
	float accuracy;

	// 반동 수치
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "4_Projectile|Aim")
	float recoil;

	UPROPERTY()
	FTimerHandle recoilHandle;

	float continuousShotCount;
	TObjectPtr<UCurveFloat> recoilCurve;

#pragma endregion

#pragma endregion


public:
#pragma region Derived From AWeaponBase

	virtual void StartAttack() override;

#pragma endregion

#pragma region Derived From IProjectileWeaponTemplate

	// 발사체를 생성하는 함수
	virtual void SpawnProjectile_Implementation(const int32 projectileAmount) override;

	// 발사될 투사체의 최초 위치, 회전, 크기 값을 무작위로 반환하는 함수
	virtual FTransform GetRandomProjectileTransform_Implementation(const FVector targetLocation) override;

	// 무기를 재장전하는 함수
	virtual void Reload_Implementation() override;

#pragma endregion

	void ApplyRecoil();
	void ResetRecoil();

protected:
#pragma region Derived From AWeaponBase

	// 무기의 공격이 시작되는 시점에 호출되는 함수 (공격 입력 이후)
	virtual void NotifyBeginAttack_Implementation() override;

	// 무기의 공격이 종료되는 시점에 호출되는 함수 (공격 입력 이후)
	virtual void NotifyEndAttack_Implementation() override;

#pragma endregion

	// 무기의 재장전이 종료되는 시점에 호출되는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Projectile Weapon")
	void NotifyEndRelaod();
	// 무기의 재장전이 종료되는 시점에 호출되는 함수
	virtual void NotifyEndRelaod_Implementation();
};
