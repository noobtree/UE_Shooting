// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "ShootingPlayerController.h"
#include "UE_Shooting/Components/InputBindable.h"
#include "UE_Shooting/Weapons/WeaponBase.h"

// Sets default values
AShootingCharacter::AShootingCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// FirstPersonMeshComponent Settings
	firstPersonMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(FName("FirstPersonMeshComponent"));
	firstPersonMeshComponent->SetupAttachment(GetMesh());
	firstPersonMeshComponent->SetOnlyOwnerSee(true);

	// FirstPersonCameraComponent Settings
	firstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("FirstPersonCameraComponent"));
	firstPersonCameraComponent->SetupAttachment(firstPersonMeshComponent);
	firstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(0, 10, 90 + BaseEyeHeight), FRotator(0, 90, 0));
	firstPersonCameraComponent->bUsePawnControlRotation = true;

	// SpringArmComponent Settings
	springArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("SpringArmComponent"));
	springArmComponent->SetupAttachment(RootComponent);
	springArmComponent->SetRelativeLocation(FVector(0, 0, BaseEyeHeight));
	springArmComponent->TargetArmLength = 400;
	springArmComponent->bUsePawnControlRotation = true;

	// ThirdPersonCameraComponent Settings
	thirdPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(FName("ThirdPersonCameraComponent"));
	thirdPersonCameraComponent->SetupAttachment(springArmComponent);

	// Extra Settings
	USkeletalMeshComponent* thirdPersonMeshComponent = GetMesh();
	thirdPersonMeshComponent->SetRelativeLocationAndRotation(FVector(0, 0, -90), FRotator(0, -90, 0));
	thirdPersonMeshComponent->SetOwnerNoSee(true);

	// rotation Axis setting
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	
	viewpoint = ECharacterViewpoint::FirstPerson;

#pragma region First Person Viewpoint Settings
	
	// 3인칭 시점 카메라 비활성화
	thirdPersonCameraComponent->SetActive(false);

	// 이동 방향으로 캐릭터 회전 옵션 활성화
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// SkeletalMeshComponent 설정
	GetMesh()->SetOwnerNoSee(true);	// 3인칭 캐릭터가 보이도록 설정
	firstPersonMeshComponent->SetOnlyOwnerSee(true);	// 1인칭 캐릭터가 보이지 않도록 설정

#pragma endregion
}

// Called when the game starts or when spawned
void AShootingCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShootingCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShootingCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AShootingPlayerController* playerController = CastChecked<AShootingPlayerController>(Controller))
	{
		if (UEnhancedInputComponent* enhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if (playerController->moveInputAction != nullptr)
			{
				enhancedInputComponent->BindAction(playerController->moveInputAction, ETriggerEvent::Triggered, this, &AShootingCharacter::MoveAction);
			}
			if (playerController->jumpInputAction != nullptr)
			{
				enhancedInputComponent->BindAction(playerController->jumpInputAction, ETriggerEvent::Started, this, &ACharacter::Jump);
				enhancedInputComponent->BindAction(playerController->jumpInputAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
			}
			if (playerController->lookInputAction != nullptr)
			{
				enhancedInputComponent->BindAction(playerController->lookInputAction, ETriggerEvent::Triggered, this, &AShootingCharacter::LookAction);
			}
			if (playerController->changeViewpointInputAction != nullptr)
			{
				enhancedInputComponent->BindAction(playerController->changeViewpointInputAction, ETriggerEvent::Started, this, &AShootingCharacter::ChangeViewpointAction);
			}
			if (playerController->attackInputAction != nullptr)
			{
				enhancedInputComponent->BindAction(playerController->attackInputAction, ETriggerEvent::Started, this, &AShootingCharacter::AttackStartedAction);
				enhancedInputComponent->BindAction(playerController->attackInputAction, ETriggerEvent::Completed, this, &AShootingCharacter::AttackCompletedAction);
			}
		}
	}

	TArray<UActorComponent*> inputBindableComponents = GetComponentsByInterface(UInputBindable::StaticClass());
	for (UActorComponent* component : inputBindableComponents)
	{
		if (IsValid(component) == true)
		{
			if (IInputBindable* inputBindableComponent = Cast<IInputBindable>(component))
			{
				inputBindableComponent->SetupPlayerInputComponent(PlayerInputComponent);
			}
		}
	}
}

void AShootingCharacter::SetCharacterViewPoint(ECharacterViewpoint newViewpoint)
{
	viewpoint = newViewpoint;

	// 1인칭 설정
	if (viewpoint == ECharacterViewpoint::FirstPerson)
	{
		// 1인칭 시점 카메라 활성화
		firstPersonCameraComponent->SetActive(true);
		// 3인칭 시점 카메라 비활성화
		thirdPersonCameraComponent->SetActive(false);

		// 컨트롤러의 Yaw 회전 입력이 액터에 적용되도록 설정
		bUseControllerRotationYaw = true;

		// 이동 방향으로 캐릭터 회전 옵션 비활성화
		GetCharacterMovement()->bOrientRotationToMovement = false;

		// SkeletalMeshComponent 설정
		GetMesh()->SetOwnerNoSee(true);		// 3인칭 캐릭터가 보여지지 않도록 설정
		firstPersonMeshComponent->SetOnlyOwnerSee(true);	// 1인칭 캐릭터가 보여지도록 설정
		return;
	}
	// 3인칭 설정
	if (viewpoint == ECharacterViewpoint::ThirdPerson)
	{
		// 3인칭 시점 카메라 활성화
		thirdPersonCameraComponent->SetActive(true);
		// 1인칭 시점 카메라 비활성화
		firstPersonCameraComponent->SetActive(false);

		// 컨트롤러의 Yaw 회전 입력이 액터에 적용되지 않도록 설정
		bUseControllerRotationYaw = false;

		// 이동 방향으로 캐릭터 회전 옵션 활성화
		GetCharacterMovement()->bOrientRotationToMovement = true;

		// SkeletalMeshComponent 설정
		GetMesh()->SetOwnerNoSee(false);		// 3인칭 캐릭터가 보여지도록 설정
		firstPersonMeshComponent->SetOnlyOwnerSee(false);	// 1인칭 캐릭터가 보여지지 않도록 설정
	}
}

FHitResult AShootingCharacter::GetInformationLookingAt(float distance)
{
	UCameraComponent* camera = viewpoint == ECharacterViewpoint::FirstPerson ? firstPersonCameraComponent : thirdPersonCameraComponent;

	FHitResult hitResult;
	FVector startLocation = camera->GetComponentLocation();
	FVector endLocation = startLocation + distance * camera->GetForwardVector();
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(hitResult, startLocation, endLocation, ECollisionChannel::ECC_Visibility, queryParams);

	return hitResult;
}

#pragma region Derived From IWeaponHolderTemplate

bool AShootingCharacter::AddWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass, FInstancedStruct& deltaPropery, bool bIsSwap)
{
	// nullptr 방어
	if (weaponClass == nullptr)
	{
		return false;
	}

	UClass* addSuperClass = weaponClass->GetSuperClass();

	// 중복된 유형의 무기 검색 (주무기, 보조무기, 투척무기 등)
	TSubclassOf<AWeaponBase> duplicatedKey = nullptr;
	for (const auto& pair : ownedWeapons)
	{
		if (pair.Key != nullptr)
		{
			UClass* existSupperClass = pair.Key->GetSuperClass();

			if (addSuperClass == existSupperClass)
			{
				duplicatedKey = pair.Key;
				break;
			}
		}
	}

	// 중복된 유형의 무기가 존재하는 경우
	if (duplicatedKey != nullptr)
	{
		// 무기를 교체하지 않는 경우
		if (bIsSwap == false)
		{
			// 함수 조기 종료
			return false;
		}

		// 보유 목록에서 해당 무기 제거
		IWeaponHolderTemplate::Execute_RemoveWeaponClass(this, duplicatedKey);
	}

	// 보유 무기 목록에 새로운 무기 추가
	ownedWeapons.Add(weaponClass, deltaPropery);

	return true;
}

bool AShootingCharacter::RemoveWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass)
{
	// nullptr 방어
	if (weaponClass == nullptr)
	{
		return false;
	}
	// 현재 장착중인 무기를 제거하는 경우
	if (holdWeapon->GetClass() == weaponClass->GetClass())
	{
		// 무기 해제
		IWeaponHolderTemplate::Execute_DisarmWeapon(this, weaponClass);
	}

	// 보유 목록에서 무기 제거 후 CDO와의 차이값 얻기
	FInstancedStruct deltaProperty;
	if (ownedWeapons.RemoveAndCopyValue(weaponClass, deltaProperty) == true)
	{
		// TODO :
		// Ex) 아이템 액터 생성

		return true;
	}

	return false;
}

void AShootingCharacter::ArmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass)
{
	// 무기 클래스가 유효하지 않은 경우
	// 해당 클래스의 무기를 보유하지 않은 경우
	if (weaponClass == nullptr || ownedWeapons.Contains(weaponClass) == false)
	{
		// 함수 조기 종료
		return;
	}

	// 현재 장착한 무기가 존재하는 경우
	if (holdWeapon != nullptr)
	{
		// 현재 장착한 무기가 교체하려는 무기와 동일한 경우
		if (holdWeapon->GetClass() == weaponClass)
		{
			// 함수 조기 종료
			return;
		}

		IWeaponHolderTemplate::Execute_DisarmWeapon(this, holdWeapon->GetClass());
	}

	// 스폰 옵션 설정
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = this;
	spawnParams.Instigator = this;

	// 무기 클래스 인스턴스 생성
	AWeaponBase* weapon = GetWorld()->SpawnActor<AWeaponBase>(weaponClass, GetActorTransform(), spawnParams);

	// CDO와 비교하여 변화된 속성값 적용
	FInstancedStruct deltaProperty = ownedWeapons[weaponClass];
	weapon->ApplyDeltaProperty(deltaProperty);

	SetHoldWeapon(weapon);
}

void AShootingCharacter::DisarmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass)
{
	// 잘못된 무기 클래스 또는 미 보유한 무기의 경우
	if (weaponClass == nullptr || ownedWeapons.Contains(weaponClass) == false)
	{
		// 함수 조기 종료
		return;
	}

	// 현재 장착한 무기가 존재하는 경우
	if (holdWeapon != nullptr)
	{
		// 현재 장착한 무기가 무장 해제하려는 무기가 동일한 경우
		if (holdWeapon->GetClass() == weaponClass)
		{
			// CDO와 비교하여 변화된 속성값 추출
			FInstancedStruct deltaProperty = holdWeapon->ExtractDeltaProperty();

			// 변화된 속성값 저장
			ownedWeapons[weaponClass] = deltaProperty;

			// 무기 객체 파괴
			holdWeapon->Destroy();
			holdWeapon = nullptr;
		}
	}
}

void AShootingCharacter::PlayWeaponMontage_Implementation(UAnimMontage* firstPersonMontage, UAnimMontage* thirdPersonMontage)
{
	if (firstPersonMeshComponent != nullptr && firstPersonMontage != nullptr)
	{
		if (UAnimInstance* animInstance1P = firstPersonMeshComponent->GetAnimInstance())
		{
			float duration = animInstance1P->Montage_Play(firstPersonMontage);
		}
	}
	if (GetMesh() != nullptr && thirdPersonMontage != nullptr)
	{
		if (UAnimInstance* animInstance3P = GetMesh()->GetAnimInstance())
		{
			float duration = animInstance3P->Montage_Play(thirdPersonMontage);
		}
	}
	return;
}

void AShootingCharacter::UpdateWeaponHUDWidget_Implementation(AWeaponBase* weaponActor)
{
}

FVector AShootingCharacter::GetWeaponTargetLocation_Implementation()
{
	UCameraComponent* Camera = viewpoint == ECharacterViewpoint::FirstPerson ? firstPersonCameraComponent : thirdPersonCameraComponent;

	FHitResult hit;
	FVector startLocation = Camera->GetComponentLocation();
	FVector endLocation = startLocation + 50000 * Camera->GetForwardVector();	// 500m 검사
	FCollisionQueryParams queryParams;
	queryParams.AddIgnoredActor(this);
	
	bool bIsHit = GetWorld()->LineTraceSingleByChannel(hit, startLocation, endLocation, ECC_Visibility, queryParams);

	if (bIsHit == true && hit.bBlockingHit == true)
	{
		return hit.ImpactPoint;
	}
	return endLocation;
}

#pragma endregion

#pragma region Inputs

void AShootingCharacter::MoveAction(const FInputActionValue& value)
{
	if (Controller == nullptr)
	{
		return;
	}

	// 입력 값을 FVector 타입으로 얻기
	FVector inputVector = value.Get<FVector>();
	if (inputVector.IsNearlyZero() == false)
	{
		// 컨트롤러의 회전값 얻기
		FRotator controlRotation = GetControlRotation();

		// 컨트롤러 회전을 기준으로 방향 조정
		FVector direction = inputVector.RotateAngleAxis(controlRotation.Yaw, FVector::UpVector);

		// 이동 입력 대입
		AddMovementInput(direction);
	}
}

void AShootingCharacter::LookAction(const FInputActionValue& value)
{
	if (Controller == nullptr)
	{
		return;
	}

	// 입력 값을 FVector 타입으로 얻기
	FVector inputVector = value.Get<FVector>();
	if (inputVector.IsNearlyZero() == false)
	{
		AddControllerYawInput(inputVector.X);
		AddControllerPitchInput(inputVector.Y);
	}
}

void AShootingCharacter::ChangeViewpointAction(const FInputActionValue& value)
{
	if (Controller == nullptr)
	{
		return;
	}

	if (viewpoint == ECharacterViewpoint::FirstPerson)
	{
		SetCharacterViewPoint(ECharacterViewpoint::ThirdPerson);
	}
	else
	{
		SetCharacterViewPoint(ECharacterViewpoint::FirstPerson);
	}
}

void AShootingCharacter::AttackStartedAction()
{
	if (Controller == nullptr || holdWeapon == nullptr)
	{
		return;
	}

	holdWeapon->StartAttack();
}

void AShootingCharacter::AttackCompletedAction()
{
	if (Controller == nullptr || holdWeapon == nullptr)
	{
		return;
	}

	holdWeapon->StopAttack();
}

void AShootingCharacter::SetHoldWeapon(AWeaponBase* weapon)
{
	if (weapon == nullptr)
	{
		return;
	}

	// 무기 액터 부착
	weapon->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	// 액터의 SkeletalMeshComponent에 무기 MeshComponent 부착 및 애니메이션 그래프 설정
	if (firstPersonMeshComponent != nullptr)
	{
		weapon->GetFirstPersonMeshComponent()->AttachToComponent(firstPersonMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
		firstPersonMeshComponent->SetAnimInstanceClass(weapon->GetFirstPersonAnimInstance());
	}
	if (USkeletalMeshComponent* characterMeshComponent = GetMesh())
	{
		weapon->GetThirdPersonMeshComponent()->AttachToComponent(characterMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
		characterMeshComponent->SetAnimInstanceClass(weapon->GetThirdPersonAnimInstance());
	}

	holdWeapon = weapon;
}

#pragma endregion

