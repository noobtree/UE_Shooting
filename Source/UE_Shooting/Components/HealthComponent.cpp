// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"
#include "PlayerManager.h"
#include "ShootingPlayerController.h"
#include "UE_Shooting/UI/HeadUpDisplayWidget.h"
#include "UE_Shooting/UI/HealthWidget.h"
#include "UE_Shooting/Managers/LocalWidgetManager.h"
#include "UE_Shooting/UI/WidgetAcceptable.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	// 체력 설정
	InitializeHealthProperty();

	// TakeDamage 연동
	if (AActor* owner = GetOwner())
	{
		owner->OnTakeAnyDamage.AddDynamic(this, &UHealthComponent::OnTakeAnyDamage);

		ULocalWidgetManager* localWidgetManager = ULocalWidgetManager::GetLocalWidgetManager(this);
		if (localWidgetManager != nullptr)
		{
			UUserWidget* hudWidgetInstance = localWidgetManager->FindOrAddWidget(hudWidgetClass);
			UHealthWidget* healthWidget = Cast<UHealthWidget>(CreateWidget(localWidgetManager->GetLocalPlayer()->PlayerController, widgetClass));
			
			if (hudWidgetInstance != nullptr && healthWidget != nullptr)
			{
				IWidgetAcceptable::Execute_AddChildWidget(hudWidgetInstance, healthWidget);
				OnHealthChanged.AddDynamic(healthWidget, &UHealthWidget::OnHealthChanged);
			}
		}
	}
}

void UHealthComponent::OnComponentDestroyed(bool bDestroyingHierarchy)
{
	// 현재 체력 상황 저장
	EntrustHealthProperty();
}


void UHealthComponent::ModifiyMaxHealth(const float deltaMaxHealth)
{
	maxHealth += deltaMaxHealth;
	if (maxHealth < 1)
	{
		maxHealth = 1;
	}
	if (OnHealthChanged.IsBound() == true)
	{
		OnHealthChanged.Broadcast(currentHealth, maxHealth);
	}
}

void UHealthComponent::InitializeHealthProperty()
{
	// GameInstance 얻기
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance != nullptr)
	{
		// GameInstance의 SubSystem 얻기 (플레이어 관리 매니저)
		UPlayerManager* manager = GameInstance->GetSubsystem<UPlayerManager>();

		// 현재 체력과 최대체력 불러오기
		if (manager->bIsHealthSaved == true)
		{
			manager->bIsHealthSaved = false;
			maxHealth = manager->playerHealth.maxHealth;
			currentHealth = manager->playerHealth.currentHealth;
			return;
		}
	}

	// 초기 체력으로 설정
	maxHealth = 100;
	currentHealth = maxHealth;
	return;
}

void UHealthComponent::EntrustHealthProperty()
{
	// GameInstance 얻기
	UWorld* world = GetWorld();
	if (world == nullptr)
	{
		return;
	}
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance != nullptr)
	{
		// GameInstance의 SubSystem 얻기 (플레이어 관리 매니저)
		UPlayerManager* manager = GameInstance->GetSubsystem<UPlayerManager>();

		// 현재 체력과 최대체력 저장
		manager->bIsHealthSaved = true;
		manager->playerHealth.maxHealth = maxHealth;
		manager->playerHealth.currentHealth = bIsAlive == true ? currentHealth : maxHealth;		// 사망한 경우 최대 체력을 현재 체력으로 저장
	}
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{

	// 잔여 체력 확인
	// 무적 상태 확인
	if (bIsAlive == false || bIsInvincible == true)
	{
		return;
	}

	if (DamageType != nullptr)
	{
		// 데미지 타입 내부에 정의된 공식에 따라 데미지 계산
		// Damage = DamageType->CalculateDamage(Damage this);
	}
	else
	{
		// 데미지 최소값 보정
		Damage = FMath::Max(0, Damage);
		currentHealth -= Damage;
	}
	// 데미지 최소값 보정
	Damage = FMath::Max(0, Damage);
	currentHealth -= Damage;

	// 체력 최솟값 보정
	currentHealth = currentHealth < 0 ? 0 : currentHealth;

	// 변경된 체력에 대한 이벤트 실행
	if (OnHealthChanged.IsBound() == true)
	{
		OnHealthChanged.Broadcast(currentHealth, maxHealth);
	}

	// 치명상 확인
	if (currentHealth <= 0 && OnRequestDeath.IsBound() == true)
	{
		OnRequestDeath.Broadcast(InstigatedBy, DamageCauser);
	}

	return;
}

void UHealthComponent::KillActor(AController* InstigatedBy, AActor* KillCauser)
{
	bIsAlive = false;
	if (OnDeath.IsBound() == true)
	{
		OnDeath.Broadcast(InstigatedBy, KillCauser);
	}
}

void UHealthComponent::ReviveActor(AController* InstigatedBy, AActor* ReviveCauser)
{
	bIsAlive = true;
	if (OnRevive.IsBound() == true)
	{
		OnRevive.Broadcast(InstigatedBy, ReviveCauser);
	}
}

void UHealthComponent::HealActor(float HealAmount, UDamageType* HealType, AController* InstigatedBy, AActor* HealCauser)
{
	if (bIsAlive == false)
	{
		return;
	}

	if (HealType != nullptr)
	{
		// 데미지 타입 내부에 정의된 공식에 따라 체력 회복
		// HealType->ApplyHeal(this);
	}
	else
	{
		// 체력 회복 최소값 보정
		HealAmount = FMath::Max(0, HealAmount);
		currentHealth += HealAmount;
	}

	// 체력 최대값 보정
	currentHealth = currentHealth > maxHealth ? maxHealth : currentHealth;

	// 변경된 체력에 대한 이벤트 실행
	OnHealthChanged.Broadcast(currentHealth, maxHealth);

	return;
}

