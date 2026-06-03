// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TimelineComponent.h"
#include "HealthWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE_SHOOTING_API UHealthWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	// UMG에서의 BeginPlay 역할을 하는 함수(Construct)에 대한 C++ 초기화 함수
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
#pragma region Components

	// 현재 체력 상태를 표시하는 TextBlock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class UTextBlock> healthTextBlock;

	// 좌측 체력바의 크기를 조절하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class USizeBox> leftBarSizeBox;

	// 좌측 현재 체력 상태에 대한 체력바의 크기를 조절하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class USizeBox> leftHealthSizeBox;

	// 좌측 최근 데미지에 대한 체력바의 크기를 조절하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class USizeBox> leftDamageTrailSizeBox;

	// 우측 체력바의 크기를 조절하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class USizeBox> rightBarSizeBox;

	// 우측 현재 체력 상태에 대한 체력바의 크기를 조절하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class USizeBox> rightHealthSizeBox;

	// 우측 최근 데미지에 대한 체력바의 크기를 조절하는 SizeBox
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget, AllowPrivateAccess = "true"), Category = "Health Widget")
	TObjectPtr<class USizeBox> rightDamageTrailSizeBox;

#pragma endregion

	// 체력바의 폭
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health Widget")
	float healthBarMaxWidth;

	// 데미지 피격 시 체력바에 표시되는 데미지 부분에 대한 동작을 정의한 커브 에셋
	// Value == 0 : 데미지 전체 표시 || Value == 1 데미지 표시 숨김
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health Widget|DamageTrail")
	TObjectPtr<UCurveFloat> damageTrailShrinkCurve;

	// 최근 데미지에 대한 체력바의 폭
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health Widget|DamageTrail")
	float damageTrailWidth;

	// 현재 체력에 대한 체력바의 폭
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Health Widget|DamageTrail")
	float healthBarWidth;

	// 데미지 트레일의 폭이 줄어드는 시작 타이밍을 관리하는 핸들
	UPROPERTY()
	FTimerHandle damageTrailTimerHandle;

	// 데미지 트레깅릐 폭이 줄어드는 속도를 관리하는 타임라인
	UPROPERTY()
	FTimeline damageTrailTimeline;

public:
	UFUNCTION(BlueprintCallable)
	virtual void OnHealthChanged(float currentHealth, float maxHealth);

protected:

	UFUNCTION(BlueprintCallable)
	virtual void StartDamageTrailTimeline();

	UFUNCTION(BlueprintCallable)
	virtual void OnTimelineUpdate(float interpFactor);
};
