// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ObjectPoolParams.h"
#include "SingleObjectPool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FObjectPoolDisposeSignature);

class UObjectPoolSubjectComponent;

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API USingleObjectPool : public UObject
{
	GENERATED_BODY()

public:
	virtual void BeginDestroy() override;

public:
	// ObjectPool에서 모든 객체들을 회수할 때 발생하는 이벤트
	UPROPERTY(BlueprintAssignable)
	FObjectPoolDisposeSignature OnDispose;

	// ObjectPool 설정 값
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FObjectPoolParams params;

protected:
	// 객체를 보관하는 컨테이너
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	TArray<TObjectPtr<UObjectPoolSubjectComponent>> pool;

	// TArray를 사용한 Queue 동작을 위한 인덱스
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	int32 headIndex = 0;

public:
	// ObjectPool 초기화
	// 정해진 수량만큼 액터 생성 및 보관
	UFUNCTION(BlueprintCallable)
	void InitializeObjectPool();

	//ObjectPool에 객체의 인스턴스를 꺼내오는 함수
	UFUNCTION(BlueprintCallable)
	UObjectPoolSubjectComponent* GetSubject();

	// 객체의 인스턴스를 ObjectPool에 반환하는 함수
	UFUNCTION(BlueprintCallable)
	void ReleaseSubject(UObjectPoolSubjectComponent* subject);

	// ObjectPool에 남아있는 모든 객체와 꺼내어진 객체들을 반환받는 함수
	UFUNCTION(BlueprintCallable)
	void Dispose();

	// ObjectPool이 비어있는지 확인하는 함수
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsEmpty() const { return headIndex >= pool.Num(); }

protected:
	// ObjectPoolParams에서 설정한 클래스에 해당하는 액터 생성 후 ObjectPool 관리 컴포넌트를 부착시켜 반환하는 함수
	UFUNCTION(BlueprintCallable)
	UObjectPoolSubjectComponent* CreateSubject();

	// ObjectPool에서 객체를 받았을 경우 실행되는 함수
	UFUNCTION(BlueprintCallable)
	void OnGetFromObjectPool(UObjectPoolSubjectComponent* subject);

	// ObjectPool에 객체를 반환하는 경우 실행되는 함수
	UFUNCTION(BlueprintCallable)
	void OnReleaseToObjectPool(UObjectPoolSubjectComponent* subject);
};