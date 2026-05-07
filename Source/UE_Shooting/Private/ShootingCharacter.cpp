// Fill out your copyright notice in the Description page of Project Settings.


#include "ShootingCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputComponent.h"
#include "ShootingPlayerController.h"
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

#pragma region Derived From IWeaponHolderTemplate

void AShootingCharacter::AddWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass)
{
	// 동일한 무기 검색
	AWeaponBase** duplicatedWeapon = ownedWeapons.FindByPredicate([weaponClass](const AWeaponBase* weapon)
		{
			return weapon != nullptr && weapon->GetClass() == weaponClass;
		});
	
	// 중복되는 무기가 존재하지 않는 경우
	if (duplicatedWeapon == nullptr)
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		spawnParams.Instigator = this;

		// 무기 생성
		AWeaponBase* weapon = GetWorld()->SpawnActor<AWeaponBase>(weaponClass, GetActorTransform(), spawnParams);

		// 보유 목록에 무기 추가
		ownedWeapons.Add(weapon);

		// 생성된 무기를 캐릭터에게 부착
		IWeaponHolderTemplate::Execute_AttachWeaponMeshes(this, weapon, weapon->GetFirstPersonMeshComponent(), weapon->GetThirdPersonMeshComponent());
	}
}

void AShootingCharacter::AttachWeaponMeshes_Implementation(AWeaponBase* weaponActor, USkeletalMeshComponent* firstPersonWeaponMesh, USkeletalMeshComponent* thirdPersonWeaponMesh)
{
	// 액터 부착
	weaponActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	weaponActor->SetActorEnableCollision(false);

	// 1인칭 Mesh 부착
	if (firstPersonMeshComponent != nullptr)
	{
		if (firstPersonMeshComponent->DoesSocketExist(FName("HandGrip_R")) == true)
		{
			firstPersonWeaponMesh->AttachToComponent(firstPersonMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandGrip_R"));
		}
		firstPersonWeaponMesh->SetCollisionProfileName(FName("NoCollision"));
	}
	// 3인칭 Mesh 부착
	if (USkeletalMeshComponent* thirdPersonMeshComponent = GetMesh())
	{
		thirdPersonWeaponMesh->AttachToComponent(thirdPersonMeshComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("HandGrip_R"));
		thirdPersonWeaponMesh->SetCollisionProfileName(FName("NoCollision"));
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

void AShootingCharacter::ActivateWeapon_Implementation(AWeaponBase* weaponActor)
{
	if (firstPersonMeshComponent != nullptr)
	{
		firstPersonMeshComponent->SetAnimInstanceClass(weaponActor->GetFirstPersonAnimInstance());
	}
	if (USkeletalMeshComponent* thirdPersonMeshComponent = GetMesh())
	{
		thirdPersonMeshComponent->SetAnimInstanceClass(weaponActor->GetThirdPersonAnimInstance());
	}

	IWeaponHolderTemplate::Execute_UpdateWeaponHUDWidget(this, weaponActor);
}

void AShootingCharacter::DeativateWeapon_Implementation(AWeaponBase* weaponActor)
{
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
	if (Controller == nullptr || ownedWeapons.IsValidIndex(equipedIndex) == false)
	{
		return;
	}

	if (AWeaponBase* equipedWeapon = ownedWeapons[equipedIndex])
	{
		equipedWeapon->StartAttack();
	}
}

void AShootingCharacter::AttackCompletedAction()
{
	if (Controller == nullptr || ownedWeapons.IsValidIndex(equipedIndex) == false)
	{
		return;
	}

	if (AWeaponBase* equipedWeapon = ownedWeapons[equipedIndex])
	{
		equipedWeapon->StopAttack();
	}
}

#pragma endregion

