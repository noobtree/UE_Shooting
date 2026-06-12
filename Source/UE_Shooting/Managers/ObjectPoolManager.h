// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SingleObjectPool.h"
#include "ObjectPoolManager.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UObjectPoolManager : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Deinitialize() override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, USingleObjectPool*> pools;

public:
	bool RegisterObjectPoolSubject(const FObjectPoolParams& poolParam);

	template<typename TActor = AActor>
	bool RegisterObjectPoolSubject(const FName& subjectName, const TSubclassOf<AActor>& subjectTemplate = TActor::StaticClass(), int32 capacity = 1, bool bAllowOverCapacity = false)
	{
		static_assert(TIsDerivedFrom<TActor, AActor>::Value, "T must be class derived from AActor");

		if (IsValid(subjectTemplate) == false || capacity <= 0 || subjectName.IsNone() == true)
		{
			return false;
		}

		FObjectPoolParams poolParam = FObjectPoolParams(subjectName, subjectTemplate, capacity, bAllowOverCapacity);
		return RegisterObjectPoolSubject(poolParam);
	}

	AActor* GetSubjectActor(const FName& subjectName);

	AActor* GetSubjectActor(const FName& subjectName, const FTransform worldTransform);

	AActor* GetSubjectActor(const FName& subjectName, const FVector& worldLocation, const FRotator worldRotation);

	template<typename TActor = AActor>
	TActor* GetSubjectActor(const FName& subjectName)
	{
		static_assert(TIsDerivedFrom<TActor, AActor>::Value, "T must be class derived from AActor");

		AActor* subjectActor = GetSubjectActor(subjectName);
		if (IsValid(subjectActor) == true)
		{
			return Cast<TActor>(subjectActor);
		}
		return nullptr;
	}

	template<typename TActor = AActor>
	TActor* GetSubjectActor(const FName& subjectName, const FTransform worldTransform)
	{
		static_assert(TIsDerivedFrom<TActor, AActor>::Value, "T must be class derived from AActor");

		AActor* subjectActor = GetSubjectActor(subjectName, worldTransform);
		if (IsValid(subjectActor) == true)
		{
			return Cast<TActor>(subjectActor);
		}
		return nullptr;
	}

	template<typename TActor = AActor>
	TActor* GetSubjectActor(const FName& subjectName, const FVector& worldLocation, const FRotator worldRotation)
	{
		static_assert(TIsDerivedFrom<TActor, AActor>::Value, "T must be class derived from AActor");

		AActor* subjectActor = GetSubjectActor(subjectName, worldLocation, worldRotation);
		if (IsValid(subjectActor) == true)
		{
			return Cast<TActor>(subjectActor);
		}
		return nullptr;
	}

	void ReleaseSubject(AActor* subjectActor);

	void ReleaseSubject(UObjectPoolSubjectComponent* subject);
};
