// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UE_Shooting/Weapons/WeaponHolderTemplate.h"
#include "ShootingCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;

struct FInputActionValue;

UENUM()
enum class ECharacterViewpoint : uint8
{
	FirstPerson UMETA(DisplayName = "First Person View"),
	ThirdPerson UMETA(DisplayName = "Third Person View")
};

UCLASS()
class UE_SHOOTING_API AShootingCharacter : public ACharacter, public IWeaponHolderTemplate
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShootingCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
#pragma region Components

	// 1인칭 시점 캐릭터 메쉬 컴포넌트
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USkeletalMeshComponent> firstPersonMeshComponent;

	// 1인칭 시점을 보여주는 카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> firstPersonCameraComponent;

	// 3인칭 시점 카메라가 붙어있는 SpringArm
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<USpringArmComponent> springArmComponent;

	// 3인칭 시점을 보여주는 카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = true))
	TObjectPtr<UCameraComponent> thirdPersonCameraComponent;

#pragma endregion

#pragma region Weapon

	// 보유한 무기 목록
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Weapon")
	TMap<TSubclassOf<AWeaponBase>, FInstancedStruct> ownedWeapons;

	// 현재 장착중인 무기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Weapon")
	AWeaponBase* holdWeapon;

#pragma endregion

	// 1인칭 시점인지를 나타내는 플래그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewpoint")
	ECharacterViewpoint viewpoint = ECharacterViewpoint::FirstPerson;

public:
	void SetCharacterViewPoint(ECharacterViewpoint newViewpoint);

	// 현재 바라보고 있는 방향의 정보를 얻는 함수
	FHitResult GetInformationLookingAt(float distance = 1000);

#pragma region Derived From IWeaponHolderTemplate
	// 임의 무기를 획득하는 함수
	virtual bool AddWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass, FInstancedStruct& deltaPropery, bool bIsSwap = false) override;

	// 임의 무기를 보유 목록에서 제거하는 함수
	virtual bool RemoveWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass) override;

	// 임의 무기를 장착(부착)하는 함수
	virtual void ArmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass) override;

	// 임의 타입의 무기를 무장 해제하는 함수
	virtual void DisarmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass) override;

	// 무기를 이용한 공격에 대하여 Montage를 재생하는 함수
	virtual void PlayWeaponMontage_Implementation(UAnimMontage* firstPersonMontage, UAnimMontage* thirdPersonMontage) override;

	// 장비하고있는 무기의 상황을 UI에 업데이트하는 함수
	virtual void UpdateWeaponHUDWidget_Implementation(AWeaponBase* weaponActor) override;

	// 장비하고있는 무기를 이용하여 공격되는 지점(좌표)를 반환하는 함수
	virtual FVector GetWeaponTargetLocation_Implementation() override;

#pragma endregion

protected:
#pragma region Inputs

	void MoveAction(const FInputActionValue& value);
	void LookAction(const FInputActionValue& value);
	void ChangeViewpointAction(const FInputActionValue& value);

	void AttackStartedAction();
	void AttackCompletedAction();

#pragma endregion

	void SetHoldWeapon(AWeaponBase* weapon);
};
