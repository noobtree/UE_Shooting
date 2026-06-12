// Fill out your copyright notice in the Description page of Project Settings.


#include "SingleObjectPool.h"
#include "Components/ObjectPoolSubjectComponent.h"

void USingleObjectPool::BeginDestroy()
{
	// ObjectPool이 소멸하기 전에 소환된 전체 객체들 반환받기
	Dispose();

	Super::BeginDestroy();
}

void USingleObjectPool::InitializeObjectPool()
{
	for (int32 i = 0; i < params.capacity; ++i)
	{
		// 객체 생성
		UObjectPoolSubjectComponent* subject = CreateSubject();

		// 컴포넌트가 붙어있는 액터 얻기
		AActor* subjectActor = subject->GetOwner();
		if (IsValid(subjectActor) == true)
		{
			// 기초 Visible, Collision, Tick, Transform 처리
			subjectActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			subjectActor->SetActorTickEnabled(false);
			subjectActor->SetActorEnableCollision(false);
			subjectActor->SetActorHiddenInGame(true);
		}

		// 배열에 보관
		pool.Add(subject);
	}
}

UObjectPoolSubjectComponent* USingleObjectPool::GetSubject()
{
	// ObjectPool이 비어있는지 확인
	if (IsEmpty() == false)
	{
		// 가장 앞에서 꺼내기
		UObjectPoolSubjectComponent* subject = nullptr;
		Swap(subject, pool[headIndex++]);

		// 객체 유효성 확인
		if (IsValid(subject) == false)
		{
			// 객체 생성
			subject = CreateSubject();
		}

		// 주기적인 메모리 초기화
		if (4 * headIndex > params.capacity)
		{
			pool.RemoveAt(0, headIndex);
			headIndex = 0;
		}

		OnGetFromObjectPool(subject);
		return subject;
	}

	// Capacity Over 가능성 확인
	if (params.bAllowOverCapacity == true)
	{
		// 객체 생성
		UObjectPoolSubjectComponent* subject = CreateSubject();

		OnGetFromObjectPool(subject);
		return subject;
	}

	return nullptr;
}

void USingleObjectPool::ReleaseSubject(UObjectPoolSubjectComponent* subject)
{
	// 객체 유효성 검사
	if (IsValid(subject) == false)
	{
		return;
	}

	// Release 함수 실행
	OnReleaseToObjectPool(subject);

	// capacity 검사
	if ((pool.Num() - headIndex) <= params.capacity)
	{
		// 배열에 보관
		pool.Add(subject);
	}
	else
	{
		// 붙어있는 액터가 유효하면 해당 액터 파괴
		AActor* owner = subject->GetOwner();
		if (IsValid(owner) == true)
		{
			owner->Destroy();
		}
	}
}

void USingleObjectPool::Dispose()
{
	if (OnDispose.IsBound() == true)
	{
		OnDispose.Broadcast();
	}
}

UObjectPoolSubjectComponent* USingleObjectPool::CreateSubject()
{
	// 생성할 객체 클래스 유효성 검사
	if (IsValid(params.subjectTemplate) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("Null Reference Exception for Actor Class Template of Subject on ObjectPoolParams"));
		return nullptr;
	}

	// Spawn Parameters 설정
	FActorSpawnParameters spawnParams;
	spawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;	// Collision이 겹쳐도 해당 위치에 생성
	spawnParams.Name = params.subjectName;

	// 컴포넌트를 부착할 액터 생성
	AActor* subjectActor = GetWorld()->SpawnActor<AActor>(params.subjectTemplate, FVector::ZeroVector, FRotator::ZeroRotator, spawnParams);

	// 외부 이름 재설정
	subjectActor->SetActorLabel(subjectActor->GetName());

	// 컴포넌트 인스턴스 생성
	UObjectPoolSubjectComponent* subject = NewObject<UObjectPoolSubjectComponent>(subjectActor);

	// Unreal 매니저에 컴포넌트 기능 등록
	subject->RegisterComponent();

	// 액터에 컴포넌트 인스턴스 등록
	subjectActor->AddInstanceComponent(subject);

	// 컴포넌트 설정 초기화
	subject->InitializeSubject(this);
	return subject;
}

void USingleObjectPool::OnGetFromObjectPool(UObjectPoolSubjectComponent* subject)
{
	// 객체 유효성 검사
	if (IsValid(subject) == false)
	{
		return;
	}

	// 컴포넌트가 붙어있는 액터 얻기
	AActor* subjectActor = subject->GetOwner();
	if (IsValid(subjectActor) == true)
	{
		// 기초 Visible, Collision, Tick, Transform 처리
		subjectActor->SetActorHiddenInGame(false);
		subjectActor->SetActorEnableCollision(true);
		subjectActor->SetActorTickEnabled(true);
		subjectActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	// 일괄 반환 이벤트 연결
	OnDispose.AddUniqueDynamic(subject, &UObjectPoolSubjectComponent::Release);

	// 대여 이벤트 실행
	subject->OnGetFromObjectPool();
}

void USingleObjectPool::OnReleaseToObjectPool(UObjectPoolSubjectComponent* subject)
{
	// 객체 유효성 검사
	if (IsValid(subject) == false)
	{
		return;
	}

	// 반환 이벤트 실행
	subject->OnReleaseToObjectPool();

	// 일괄 반환 이벤트 연결 해제
	OnDispose.RemoveDynamic(subject, &UObjectPoolSubjectComponent::Release);

	// 컴포넌트가 붙어있는 액터 얻기
	AActor* subjectActor = subject->GetOwner();
	if (IsValid(subjectActor) == true)
	{
		// 기초 Visible, Collision, Tick, Transform 처리
		subjectActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		subjectActor->SetActorTickEnabled(false);
		subjectActor->SetActorEnableCollision(false);
		subjectActor->SetActorHiddenInGame(true);
	}
}
