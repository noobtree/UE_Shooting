// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

// 체력이 변경되었을 때의 이벤트 유형
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChangedSignature, float, CurrentHealth, float, MaxHealth);
// 체력이 0 이하가 되는 치명적인 데미지를 받았을 때의 이벤트 유형
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRequestDeathSignature, AController*, InstigatedBy, AActor*, DamageCauser);
// 사망이 확정되었을 때의 이벤트 유형
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnDeathSignature, AController*, InstigatedBy, AActor*, DamageCauser);
// 부활이 실행 되었을 때의 이벤트 유형
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReviveSignature, AController*, InstigatedBy, AActor*, DamageCauser);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UE_SHOOTING_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;

public:
	// 현재 체력 또는 최대 체력이 변경되었을 경우 실행되는 이벤트 (float CurrentHealth, float MaxHealth)
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnHealthChangedSignature OnHealthChanged;

	// 체력이 0 이하가 되는 치명적인 데미지를 받았을 경우 실행되는 이벤트 (AController* InstigatedBy, AActor* DamageCauser)
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnRequestDeathSignature OnRequestDeath;

	// 사망하는 경우 실행되는 이벤트 (AController* InstigatedBy, AActor* DamageCauser)
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnDeathSignature OnDeath;

	// 부활하는 경우 실행되는 이벤트 (AController* InstigatedBy, AActor* DamageCauser)
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable)
	FOnReviveSignature OnRevive;

protected:
	// 무적 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	bool bIsInvincible = false;

	// 생존 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	bool bIsAlive = true;

	// 최대 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	float maxHealth;

	// 현재 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "General Settings")
	float currentHealth;

	// 체력 UI 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Settings")
	TSubclassOf<UUserWidget> widgetClass;

	// 체력 UI를 추가할 HUD UI 클래스
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "General Settings")
	TSubclassOf<UUserWidget> hudWidgetClass;

public:
	// 생존 여부를 반환하는 함수
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool IsAlive()const { return bIsAlive; }

	UFUNCTION(BlueprintCallable)
	void ModifiyMaxHealth(const float deltaMaxHealth = 0);

protected:
	// PlayerManager로부터 플레이어의 체력 정보 얻기 또는 초기화
	UFUNCTION(BlueprintCallable)
	virtual void InitializeHealthProperty();
	
	// PlayerManager에게 플레이어의 체력 정보 임시 위탁
	UFUNCTION(BlueprintCallable)
	virtual void EntrustHealthProperty();

	/// <summary>
	/// 컴포넌트를 소유한 액터가 데미지 종류과 관계없이 데미지를 받았을 때 호출되는 함수
	/// </summary>
	/// <param name="DamagedActor">데미지를 받은 액터 (일반적으로 Owner)</param>
	/// <param name="Damage">받은 Raw 데미지</param>
	/// <param name="DamageType">데미지의 종류</param>
	/// <param name="InstigatedBy">가해자 (AIController, PlayerController)</param>
	/// <param name="DamageCauser">데미지를 유발한 액터 (흉기, 무기)</param>
	UFUNCTION(BlueprintCallable)
	virtual void OnTakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	/// <summary>
	/// 컴포넌트를 소유한 액터에 대하여 사망 처리를 하는 함수
	/// </summary>
	/// <param name="InstigatedBy">가해자 (AIController, PlayerController)</param>
	/// <param name="DamageCauser">살해에 사용된 도구 액터 (흉기, 무기)</param>
	UFUNCTION(BlueprintCallable)
	virtual void KillActor(AController* InstigatedBy, AActor* KillCauser);

	/// <summary>
	/// 컴포넌트를 소유한 액터에 대하여 부활 처리를 하는 함수
	/// </summary>
	/// <param name="InstigatedBy">부활을 주도한자 (AIController, PlayerController)</param>
	/// <param name="ReviveCauser">부활에 사용된 도구 액터 (아이템, 무기)</param>
	UFUNCTION(BlueprintCallable)
	virtual void ReviveActor(AController* InstigatedBy, AActor* ReviveCauser);

	/// <summary>
	/// 컴포넌트를 소유한 액터에 대하여 회복 처리를 하는 함수
	/// </summary>
	/// <param name="HealAmount">받은 Raw 회복량</param>
	/// <param name="HealType">회복 종류</param>
	/// <param name="InstigatedBy">회복을 실행한자(AIController, PlayerController)</param>
	/// <param name="HealCauser">회복에 사용된 도구 액터 (아이템, 무기)</param>
	UFUNCTION(BlueprintCallable)
	virtual void HealActor(float HealAmount, UDamageType* HealType, AController* InstigatedBy, AActor* HealCauser);
};
