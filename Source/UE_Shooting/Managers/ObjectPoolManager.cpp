// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolManager.h"
#include "Components/ObjectPoolSubjectComponent.h"

void UObjectPoolManager::Deinitialize()
{
	for (const auto& pair : pools)
	{
		pair.Value->Dispose();
	}
	pools.Empty();
	
	Super::Deinitialize();
}

bool UObjectPoolManager::RegisterObjectPoolSubject(const FObjectPoolParams& poolParam)
{
	// 등록하려는 ObjectPool 설정 유효성 확인
	if (IsValid(poolParam.subjectTemplate) == false || poolParam.capacity <= 0 || poolParam.subjectName.IsNone() == true)
	{
		return false;
	}

	// 동일한 Key(SubjectName)으로 등록된 ObjectPool의 존재 검사
	if (pools.Contains(poolParam.subjectName) == true)
	{
		// 기존 ObjectPool의 유효성 검사
		USingleObjectPool* objectPool = pools[poolParam.subjectName];
		if (IsValid(objectPool) == true && IsValid(objectPool->params.subjectTemplate) == true)
		{
			// 기존 ObjectPool의 SubjectTemplate과 새로 등록하려는 SubjectTemplate 비교
			if (objectPool->params.subjectTemplate == poolParam.subjectTemplate)
			{
				// 등록 성공으로 간주
				return true;
			}
			// 등록 실패로 간주
			return false;
		}
		// ObjectPool 교체 진행
	}

	// ObjectPool 인스턴스 생성
	USingleObjectPool* objectPool = NewObject<USingleObjectPool>(this);

	// ObjectPool 옵션(?) 설정
	objectPool->params = poolParam;

	// 초기 객체 생성
	objectPool->InitializeObjectPool();

	// ObjectPool 목록에 등록
	pools[poolParam.subjectName] = objectPool;
	return true;
}

AActor* UObjectPoolManager::GetSubjectActor(const FName& subjectName)
{
	// 해당 키에 해당하는 ObjectPool 존재 검사
	if (pools.Contains(subjectName) == true)
	{
		// ObjectPool에서 Subject[UActorComponent] 꺼내기
		UObjectPoolSubjectComponent* subjectComponent = pools[subjectName]->GetSubject();

		// 꺼내온 Subject가 존재하면 해당 Component를 가진 액터 반환
		if (subjectComponent != nullptr)
		{
			return subjectComponent->GetOwner();
		}
	}
	return nullptr;
}

AActor* UObjectPoolManager::GetSubjectActor(const FName& subjectName, const FTransform worldTransform)
{
	AActor* subjectActor = GetSubjectActor(subjectName);

	if (IsValid(subjectActor) == true)
	{
		// 위치 및 회전값 조정
		subjectActor->SetActorTransform(worldTransform, false, nullptr, ETeleportType::ResetPhysics);

		return subjectActor;
	}

	return nullptr;
}

AActor* UObjectPoolManager::GetSubjectActor(const FName& subjectName, const FVector& worldLocation, const FRotator worldRotation)
{
	AActor* subjectActor = GetSubjectActor(subjectName);

	if (IsValid(subjectActor) == true)
	{
		// 위치 및 회전값 조정
		subjectActor->SetActorLocation(worldLocation, false, nullptr, ETeleportType::ResetPhysics);
		subjectActor->SetActorRotation(worldRotation, ETeleportType::ResetPhysics);

		return subjectActor;
	}

	return nullptr;
}

void UObjectPoolManager::ReleaseSubject(AActor* subjectActor)
{
	if (IsValid(subjectActor) == false)
	{
		return;
	}

	UObjectPoolSubjectComponent* subjectComponent = subjectActor->FindComponentByClass<UObjectPoolSubjectComponent>();
	if (IsValid(subjectComponent) == true)
	{
		subjectComponent->Release();
		return;
	}
	subjectActor->Destroy();
}

void UObjectPoolManager::ReleaseSubject(UObjectPoolSubjectComponent* subject)
{
	if (IsValid(subject) == false)
	{
		return;
	}
	subject->Release();
}
