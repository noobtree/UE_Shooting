// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "WeaponHolderTemplate.h"
#include "Components/PawnNoiseEmitterComponent.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	defaultSceneRoot = CreateDefaultSubobject<USceneComponent>(FName("DefaultSceneRootComponent"));
	SetRootComponent(defaultSceneRoot);

	firstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMeshComponent"));
	firstPersonMeshComponent->SetupAttachment(defaultSceneRoot);

	thirdPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("ThirdPersonMeshComponent"));
	thirdPersonMeshComponent->SetupAttachment(defaultSceneRoot);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
	if (Owner != nullptr)
	{
		// 무기를 보유한 액터가 특정 인터페이스를 상속받고있는지 확인
		if (Owner->GetClass()->ImplementsInterface(UWeaponHolderTemplate::StaticClass()))
		{
			// 무기를 보유한 액터에게 무기의 Mesh 붙이기
			IWeaponHolderTemplate::Execute_AttachWeaponMeshes(Owner, this, firstPersonMeshComponent, thirdPersonMeshComponent);
		}
		else
		{
			// 무기 액터 비활성화 처리
			SetActorHiddenInGame(true);
		}

		// 공격 시 소음 발생을 위하여 무기를 소유하는 액터를 Pawn으로 변환하여 저장
		ownerPawn = CastChecked<APawn>(Owner);
	}
}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::ActivateWeapon()
{
	firstPersonMeshComponent->SetHiddenInGame(false);
	thirdPersonMeshComponent->SetHiddenInGame(false);
	SetActorHiddenInGame(false);

	// UI 갱신
	IWeaponHolderTemplate::Execute_UpdateWeaponHUDWidget(ownerPawn, this);
}

void AWeaponBase::DeactivateWeapon()
{
	SetActorHiddenInGame(true);
	firstPersonMeshComponent->SetHiddenInGame(true);
	thirdPersonMeshComponent->SetHiddenInGame(true);
}

void AWeaponBase::StartAttack()
{
	// 키 입력 상태 변경
	bAttackInput = true;

	// 공격 가능 여부 확인
	if (bCanAttack == true)
	{
		// 공격 실행
		NotifyBeginAttack();
	}
	return;
}

void AWeaponBase::StopAttack()
{
	// 현재 공격이 종료된 것으로 변경
	bAttackInput = false;
}

void AWeaponBase::NotifyBeginAttack_Implementation()
{
	// 공격 입력 여부 확인
	if (bAttackInput == false)
	{
		return;
	}

	// 공격 가능 여부 확인
	if(bCanAttack == false || GetWorldTimerManager().IsTimerActive(attackTimer) == true)
	{
		return;
	}
	// 공격 가능 여부 변경
	bCanAttack = false;

	// 소유자가 공격하려는 목표 지점 얻기
	FVector targetLocation = IWeaponHolderTemplate::Execute_GetWeaponTargetLocation(ownerPawn);
	
	// 무기 소유자의 공격 모션 재생
	IWeaponHolderTemplate::Execute_PlayWeaponMontage(ownerPawn, firstPersonAttackMontage, thirdPersonAttackMontage);

	// 공격에 따른 소음 발생
	if (Owner->FindComponentByClass<UPawnNoiseEmitterComponent>() != nullptr)
	{
		MakeNoise(noiseDecibel, GetInstigator<APawn>(), GetActorLocation(), 100, FName("Attack"));
	}
}

void AWeaponBase::NotifyEndAttack_Implementation()
{
	// 공격 가능 여부 변경
	bCanAttack = true;

	// 공격 종료 시점에 공격 입력 확인
	// 무기가 자동 공격이 가능성 확인
	if (bAttackInput == true && bIsAuto == true)
	{
		// 일정 시간 지연 이후 공격 예약
		GetWorldTimerManager().SetTimer(attackTimer, this, &AWeaponBase::NotifyBeginAttack, afterAttackDelay, false);
	}
}

