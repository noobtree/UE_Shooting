// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponHolderComponent.h"

// Sets default values for this component's properties
UWeaponHolderComponent::UWeaponHolderComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UWeaponHolderComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	ownerPawn = Cast<APawn>(GetOwner());
	ownerPawn == nullptr ? SetActive(false) : SetActive(true);
}

// Called every frame
void UWeaponHolderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

bool UWeaponHolderComponent::AddWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass, FInstancedStruct& deltaPropery, bool bIsSwap)
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
		RemoveWeaponClass(duplicatedKey);
	}

	// 보유 무기 목록에 새로운 무기 추가
	ownedWeapons.Add(weaponClass, deltaPropery);

	return true;
}

bool UWeaponHolderComponent::RemoveWeaponClass_Implementation(TSubclassOf<AWeaponBase> weaponClass)
{
	// nullptr 방어
	if (weaponClass == nullptr)
	{
		return false;
	}
	// 현재 장착중인 무기를 제거하는 경우
	if (holdWeapon->GetClass() == weaponClass->GetClass())
	{
		// 무기 장착 해제
		DirarmWeapon(weaponClass);
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

void UWeaponHolderComponent::ArmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass)
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
		
		DirarmWeapon(holdWeapon->GetClass());
	}

	// 스폰 옵션 설정
	FActorSpawnParameters spawnParams;
	spawnParams.Owner = ownerPawn;
	spawnParams.Instigator = ownerPawn;

	// 무기 클래스 인스턴스 생성
	AWeaponBase* weapon = GetWorld()->SpawnActor<AWeaponBase>(weaponClass, ownerPawn->GetTransform(), spawnParams);

	// CDO와 비교하여 변화된 속성값 적용
	FInstancedStruct deltaProperty = ownedWeapons[weaponClass];
	weapon->ApplyDeltaProperty(deltaProperty);
	
	// 액터의 SkeletalMeshComponent에 무기 MeshComponent 부착
	USkeletalMeshComponent* holderMesh = ownerPawn->FindComponentByClass<USkeletalMeshComponent>();
	if (holderMesh != nullptr)
	{
		// 무기의 MeshComponent 부착
		weapon->GetFirstPersonMeshComponent()->AttachToComponent(holderMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	}

	//TArray<USkeletalMeshComponent*> meshComponents;
	//ownerPawn->GetComponents<USkeletalMeshComponent>(meshComponents);
}

void UWeaponHolderComponent::DirarmWeapon_Implementation(TSubclassOf<AWeaponBase> weaponClass)
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
