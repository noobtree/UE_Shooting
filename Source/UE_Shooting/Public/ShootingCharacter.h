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

	// 소유한 무기들
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Weapon")
	TArray<AWeaponBase*> ownedWeapons;

	// 장착한 무기의 인덱스
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Weapon")
	int32 equipedIndex;
	
	// 1인칭 시점인지를 나타내는 플래그
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Viewpoint")
	ECharacterViewpoint viewpoint = ECharacterViewpoint::FirstPerson;

public:
	void SetCharacterViewPoint(ECharacterViewpoint newViewpoint);

#pragma region Derived From IWeaponHolderTemplate
	// 임의 무기를 획득하는 함수
	virtual void AddWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass) override;

	// 임의 무기를 장착(부착)하는 함수
	virtual void AttachWeaponMeshes_Implementation(AWeaponBase* weaponActor, USkeletalMeshComponent* firstPersonWeaponMesh, USkeletalMeshComponent* thirdPersonWeaponMesh) override;

	// 무기를 이용한 공격에 대하여 Montage를 재생하는 함수
	virtual void PlayWeaponMontage_Implementation(UAnimMontage* firstPersonMontage, UAnimMontage* thirdPersonMontage) override;

	// 장비하고있는 무기의 상황을 UI에 업데이트하는 함수
	virtual void UpdateWeaponHUDWidget_Implementation(AWeaponBase* weaponActor) override;

	// 장비하고있는 무기를 이용하여 공격되는 지점(좌표)를 반환하는 함수
	virtual FVector GetWeaponTargetLocation_Implementation() override;

	// 기존에 보유하고 있으면서 비활성화 되어있는 무기를 활성화하는 함수
	virtual void ActivateWeapon_Implementation(AWeaponBase* weaponActor) override;	// 무기 교체에 사용

	// 기존에 보유하고 있으면서 활성화 되어있는 무기를 비활성화하는 함수
	virtual void DeativateWeapon_Implementation(AWeaponBase* weaponActor) override;	// 무기 교체에 사용

#pragma endregion

protected:
#pragma region Inputs

	void MoveAction(const FInputActionValue& value);
	void LookAction(const FInputActionValue& value);
	void ChangeViewpointAction(const FInputActionValue& value);

	void AttackStartedAction();
	void AttackCompletedAction();

#pragma endregion
};
