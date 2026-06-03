// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StructUtils/InstancedStruct.h"
#include "WeaponBase.generated.h"

USTRUCT(BlueprintType)
struct UE_SHOOTING_API FWeaponBaseDelta
{
	GENERATED_BODY()

public:
	// 무게 변화량
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon|Base")
	float deltaMass = 0;

	// 소음 변화량
	float deltaNoise = 0;
};

/// <summary>
/// 잡는 위치와, 총구의 위치를 socket으로 표시하기 위하여 무기의 Mesh는 USkeletalMesh를 사용
/// </summary>
UCLASS(Abstract, BlueprintType, Blueprintable)
class UE_SHOOTING_API AWeaponBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
#pragma region Components

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USceneComponent> defaultSceneRoot;

	// 1인칭 시점에서 보여지는 Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> firstPersonMeshComponent;

	// 3인칭 시점에서 보여지는 Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "1_Components", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> thirdPersonMeshComponent;

#pragma endregion

#pragma region Animations

	// 1인칭 시점 SkeletalMesh의 애니메이션 그래프가 정의된 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Animations")
	TSoftClassPtr<UAnimInstance> firstPersonAnimInstance;

	// 무기를 장착한 캐릭터의 공격 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Animations")
	TObjectPtr<UAnimMontage> firstPersonAttackMontage;

	// 3인칭 시점 SkeletalMesh의 애니메이션 그래프가 정의된 에셋
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Animations")
	TSoftClassPtr<UAnimInstance> thirdPersonAnimInstance;

	// 무기를 장착한 캐릭터의 공격 모션
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "2_Animations")
	TObjectPtr<UAnimMontage> thirdPersonAttackMontage;

#pragma endregion

#pragma region General Settings

	// 공격 입력의 여부를 판별하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "3_GeneralSettings")
	bool bAttackInput = false;

	// 현재 공격이 가능한지 판별하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "3_GeneralSettings")
	bool bCanAttack = true;

	// 자동으로 다음 공격이 가능한지를 판별하는 변수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_GeneralSettings")
	bool bIsAuto = false;

	// 공격 후 딜레이 [sec]
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_GeneralSettings")
	float afterAttackDelay = 0.1f;

	// 공격 입력이 지속적으로 들어올 때, 다음 공격을 예약하는 타이머
	UPROPERTY(BlueprintReadWrite, Category = "3_GeneralSettings")
	FTimerHandle attackTimer;

	// 공격에 따른 소음 크기
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "3_GeneralSettings")
	float noiseDecibel = 0;

#pragma endregion

	// 무기의 소유자를 저장하는 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	APawn* ownerPawn;

public:
	// 무기 활성화
	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	void ActivateWeapon();

	// 무기 비활성화
	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	void DeactivateWeapon();

	// 공격 키 입력이 시작되었을 경우 실행되는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	virtual void StartAttack();

	// 공격 키 입력이 종료되었을 경우 실행되는 함수
	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	virtual void StopAttack();

	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	virtual FInstancedStruct ExtractDeltaProperty();
	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	virtual void ApplyDeltaProperty(const FInstancedStruct deltaProperty);

#pragma region Getter

	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")	
	FORCEINLINE UClass* GetFirstPersonAnimInstance()const { return firstPersonAnimInstance.Get(); }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	FORCEINLINE USkeletalMeshComponent* GetFirstPersonMeshComponent() const { return firstPersonMeshComponent; }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")	
	FORCEINLINE UClass* GetThirdPersonAnimInstance()const { return thirdPersonAnimInstance.Get(); }

	UFUNCTION(BlueprintCallable, Category = "Weapon|Base")
	FORCEINLINE USkeletalMeshComponent* GetThirdPersonMeshComponent() const { return thirdPersonMeshComponent; }

#pragma endregion

protected:
	// 무기의 공격이 시작되는 시점에 호출되는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Base")
	void NotifyBeginAttack();
	// 무기의 공격이 시작되는 시점에 호출되는 함수
	// Ex) 총알 생성, 콜리전 생성, 몽타주 재생 등
	virtual void NotifyBeginAttack_Implementation();

	// 무기의 공격이 종료되는 시점에 호출되는 함수
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Weapon|Base")
	void NotifyEndAttack();
	// 무기의 공격이 종료되는 시점에 호출되는 함수
	// Ex) 탄피 배출, 콜리전 삭제, 몽타주 정지 등
	virtual void NotifyEndAttack_Implementation();
};
