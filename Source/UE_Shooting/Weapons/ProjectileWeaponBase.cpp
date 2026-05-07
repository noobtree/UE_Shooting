// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeaponBase.h"
#include "WeaponHolderTemplate.h"
#include "Components/PawnNoiseEmitterComponent.h"

void AProjectileWeaponBase::StartAttack()
{
	// 키 입력 상태 변경
	bAttackInput = true;

	// 공격 가능 여부 확인
	if (bCanAttack == true)
	{
		// 탄창 확인
		if (ammoInMag > 0)
		{
			// 공격 실행
			NotifyBeginAttack();
		}
		else
		{
			Reload();
		}
	}
	return;
}

void AProjectileWeaponBase::SpawnProjectile_Implementation(const int32 projectileAmount)
{
	// 소유자가 공격하려는 목표 지점 얻기
	FVector targetLocation = IWeaponHolderTemplate::Execute_GetWeaponTargetLocation(ownerPawn);

	for (int i = 0; i < projectileAmount; ++i)
	{
		// 생성 위치 얻기
		FTransform spawnTransform = IProjectileWeaponTemplate::Execute_GetRandomProjectileTransform(this, targetLocation);

		// 발사체 클래스가 유효한 경우
		if (IsValid(projectile) == true)
		{
			// 생성 옵션 설정
			FActorSpawnParameters spawnParams;
			spawnParams.Owner = ownerPawn;
			spawnParams.Instigator = ownerPawn;

			// 발사체 생성
			GetWorld()->SpawnActor(projectile.Get(), &spawnTransform, spawnParams);
		}
		// 발사체 클래스가 유효하지 않은 경우
		else
		{
			// 발사체의 궤적 그리기
			FVector startLocation = spawnTransform.GetLocation();
			FVector endLocation = startLocation + 10000 * spawnTransform.GetRotation().GetForwardVector();

			DrawDebugLine(GetWorld(), startLocation, endLocation, FColor::Red, false, 5.0f, 0, 1.0f);
		}
	}

	// 발사체 수량 감소
	--ammoInMag;

	// HUD 갱신
	IWeaponHolderTemplate::Execute_UpdateWeaponHUDWidget(ownerPawn, this);
}

FTransform AProjectileWeaponBase::GetRandomProjectileTransform_Implementation(const FVector targetLocation)
{
	const FName socketName = FName("Muzzle");
	// 소켓 존재 확인
	if (firstPersonMeshComponent->DoesSocketExist(socketName) == true)
	{
		// 소켓 위치 얻기
		FVector location = firstPersonMeshComponent->GetSocketLocation(socketName);

		// 무작위 방향 얻기
		FVector direction = targetLocation - location;
		float rad = FMath::DegreesToRadians(0.03 * (100 - accuracy));
		direction = FMath::VRandCone(direction, rad);
		
		// 무작위 방향을 바라보는 회전값 구하기
		FRotator rotator = direction.Rotation();

		// 발사체 크기 고정
		const FVector scale = FVector::OneVector;

		return FTransform(rotator, location, scale);
	}
	return FTransform();
}

void AProjectileWeaponBase::Reload_Implementation()
{
	// 공격 가능 여부 변경
	bCanAttack = false;

	// 재장전 모션 재생
	IWeaponHolderTemplate::Execute_PlayWeaponMontage(ownerPawn, firstPersonReloadMontage, thirdPersonReloadMontage);
}

void AProjectileWeaponBase::NotifyBeginAttack_Implementation()
{
	// 공격 입력 여부 확인
	// 공격 대기시간 진행 여부 확인
	if (bAttackInput == false || GetWorldTimerManager().IsTimerActive(attackTimer) == true)
	{
		return;
	}
	// 공격 가능 여부 변경
	bCanAttack = false;

	// 무기 소유자의 공격 모션 재생
	IWeaponHolderTemplate::Execute_PlayWeaponMontage(ownerPawn, firstPersonAttackMontage, thirdPersonAttackMontage);

	// 발사체 생성
	IProjectileWeaponTemplate::Execute_SpawnProjectile(this, 1);

	// 반동 적용
	ownerPawn->AddControllerPitchInput(recoil);

	// 공격에 따른 소음 발생
	if (Owner->FindComponentByClass<UPawnNoiseEmitterComponent>() != nullptr)
	{
		MakeNoise(noiseDecibel, GetInstigator<APawn>(), GetActorLocation(), 100, FName("Attack"));
	}
}

void AProjectileWeaponBase::NotifyEndAttack_Implementation()
{
	// 공격 가능 여부 변경
	bCanAttack = true;

	// 공격 종료 시점에 공격 입력 확인
	// 무기가 자동 공격이 가능성 확인
	if (bAttackInput == true && bIsAuto == true)
	{
		// 잔탄이 존재하는지 확인
		if (ammoInMag > 0)
		{
			// 일정 시간 지연 이후 공격 예약
			GetWorldTimerManager().SetTimer(attackTimer, this, &AProjectileWeaponBase::NotifyBeginAttack, afterAttackDelay, false);
		}
	}
}

void AProjectileWeaponBase::NotifyEndRelaod_Implementation()
{
	// 탄창의 여유 공간 계산
	int32 supplyAmount = magazineSize - ammoInMag;

	// 탄창이 존재하지 않거나 약실에 장전되어 있는 경우 보충 수량 추가
	supplyAmount += (magazineSize == 0 || ammoInMag > 0) ? 1 : 0;

	// 장전된 발사체 수량 증가
	ammoInMag += supplyAmount;

	// 남은 발사체 수량 감소
	remainingAmmo -= supplyAmount;

	// HUD 갱신
	IWeaponHolderTemplate::Execute_UpdateWeaponHUDWidget(ownerPawn, this);

	// 공격 가능 여부 변경
	bCanAttack = true;
}
