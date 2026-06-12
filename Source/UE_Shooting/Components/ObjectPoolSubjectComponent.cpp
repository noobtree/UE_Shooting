// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectPoolSubjectComponent.h"
#include "Managers/SingleObjectPool.h"

void UObjectPoolSubjectComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	if (observer.IsValid() == true)
	{
		// 옵저버에 설정된 이벤트 연결 해제
		observer.Get()->OnDispose.RemoveDynamic(this, &UObjectPoolSubjectComponent::Release);
	}
	observer = nullptr;
	Super::OnComponentDestroyed(bDestroyingHierarchy);
}

void UObjectPoolSubjectComponent::InitializeSubject(USingleObjectPool* objectPool)
{
	observer = objectPool;
}

void UObjectPoolSubjectComponent::Release()
{
	// Observer의 유효성 확인
	if (observer.IsValid() == true)
	{
		// 객체 반환
		observer.Get()->ReleaseSubject(this);
	}
	else
	{
		// 액터 파괴
		if (AActor* owner = GetOwner())
		{
			owner->Destroy();
		}
	}
}

void UObjectPoolSubjectComponent::OnGetFromObjectPool()
{
}

void UObjectPoolSubjectComponent::OnReleaseToObjectPool()
{
}
