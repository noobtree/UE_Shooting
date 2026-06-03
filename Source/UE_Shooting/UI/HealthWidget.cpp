// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/GridSlot.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "UE_Shooting/Components/HealthComponent.h"

void UHealthWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 게임 해상도의 가로 폭 얻기
	healthBarMaxWidth = GSystemResolution.ResX * 0.15f;

	// 양쪽 체력바의 폭 설정
	leftBarSizeBox->SetMinDesiredWidth(healthBarMaxWidth);
	leftDamageTrailSizeBox->SetWidthOverride(healthBarMaxWidth);
	rightBarSizeBox->SetMinDesiredWidth(healthBarMaxWidth);
	rightDamageTrailSizeBox->SetWidthOverride(healthBarMaxWidth);

	// 데미지 트레일 이벤트 연결
	FOnTimelineFloat timelineDelegate;
	timelineDelegate.BindDynamic(this, &UHealthWidget::OnTimelineUpdate);
	if (damageTrailShrinkCurve != nullptr)
	{
		damageTrailTimeline.AddInterpFloat(damageTrailShrinkCurve, timelineDelegate);
	}

	// 최대 체력 상태로 초기화
	OnHealthChanged(1, 1);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shrink Reserved")));

	// Widget 위치 조정
	UCanvasPanelSlot* widgetSlot = Cast<UCanvasPanelSlot>(Slot);
	if (widgetSlot != nullptr)
	{
		widgetSlot->SetAnchors(FAnchors(0.5f, 1, 0.5f, 1));		// Center-Bottom
		widgetSlot->SetAlignment(FVector2D(0.5f, 1));			// Center-Bottom
		widgetSlot->SetPosition(FVector2D(0, -45));
		widgetSlot->SetAutoSize(true);
	}
}

void UHealthWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// Timeline 시간 업데이트
	damageTrailTimeline.TickTimeline(InDeltaTime);
}

void UHealthWidget::OnHealthChanged(float currentHealth, float maxHealth)
{
	// 체력 비율 계산
	float factor = currentHealth / (maxHealth <= 0 ? 1 : maxHealth);

	// 체력바의 폭 계산
	float healthWidth = healthBarMaxWidth * factor;

	// 좌우측 체력바 크기 조정
	leftHealthSizeBox->SetWidthOverride(healthWidth);
	rightHealthSizeBox->SetWidthOverride(healthWidth);

	// 체력 텍스트 변경
	FString healthText = FString::Printf(TEXT("%.0f %%"), factor * 100);
	healthTextBlock->SetText(FText::FromString(healthText));

	StartDamageTrailTimeline();
}

void UHealthWidget::StartDamageTrailTimeline()
{
	// 데미지 트레일 UI, 현재 체력 UI의 폭 저장
	damageTrailWidth = leftDamageTrailSizeBox->GetWidthOverride();
	healthBarWidth = leftHealthSizeBox->GetWidthOverride();

	// 데미지 트레일 UI의 폭이 현재 줄어들고 있는지 확인
	if (damageTrailTimeline.IsPlaying() == false)
	{
		// 데미지 트레일 UI의 폭이 줄어들기 시작하는 타이머가 설정되어있는지 확인
		if (GetWorld()->GetTimerManager().TimerExists(damageTrailTimerHandle) == false)
		{
			// 데미지 트레일의 폭 줄어들기 예약
			FTimerDelegate timerDelegate = FTimerDelegate::CreateLambda([&]() { damageTrailTimeline.PlayFromStart(); });
			GetWorld()->GetTimerManager().SetTimer(damageTrailTimerHandle, timerDelegate, 0.1f, false);
			return;
		}
	}
	// 데미지 트레일의 폭 줄어들기 시작(처음부터)
	damageTrailTimeline.PlayFromStart();
}

void UHealthWidget::OnTimelineUpdate(float interpFactor)
{
	float width = FMath::Lerp(damageTrailWidth, healthBarWidth, interpFactor);
	leftDamageTrailSizeBox->SetWidthOverride(width);
	rightDamageTrailSizeBox->SetWidthOverride(width);
}
