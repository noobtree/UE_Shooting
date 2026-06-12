// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ObjectPoolSubjectComponent.generated.h"

class USingleObjectPool;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_SHOOTING_API UObjectPoolSubjectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

protected:
	TWeakObjectPtr<USingleObjectPool> observer;

public:
	// 객체 생성 시 최초 1회에 한하여 실행되는 함수
	// Ex) 반환될 ObjectPool 등록	
	virtual void InitializeSubject(USingleObjectPool* objectPool);

	// 객체를 ObjectPool에 반환하는 함수
	virtual void Release();

	// ObjectPool에서 객체를 받았을 경우 실행되는 함수
	virtual void OnGetFromObjectPool();

	// ObjectPool에 객체를 반환하는 경우 실행되는 함수
	virtual void OnReleaseToObjectPool();
};
