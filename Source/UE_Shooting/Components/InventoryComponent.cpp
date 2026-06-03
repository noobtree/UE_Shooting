// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "EnhancedInputComponent.h"
#include "UE_Shooting/UI/InventoryWidget.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (IsValid(IA_inventory) == true)
		{
			enhancedInputComponent->BindAction(IA_inventory, ETriggerEvent::Started, this, &UInventoryComponent::ToggleWidgetVisibility);
		}
	}
}

void UInventoryComponent::ToggleWidgetVisibility()
{
	bool bIsVisible = (IsValid(widgetInstance) == false ? false : (widgetInstance->GetVisibility() == ESlateVisibility::Visible));

	// 마우스 노출 설정
	APlayerController* localController = GetWorld()->GetFirstPlayerController<APlayerController>();
	localController->bShowMouseCursor = !bIsVisible;

	if (bIsVisible == false)
	{
		// 생성되어있는 UI 확인
		if (IsValid(widgetInstance) == false)
		{
			// 생성하려는 UI의 클래스 유효성 확인
			if (IsValid(widgetClass) == false)
			{
				UE_LOG(LogTemp, Warning, TEXT("생성하려는 UI의 클래스가 유효하지 않습니다."));
				return;
			}

			// UI 인스턴스 생성
			widgetInstance = CreateWidget<UUserWidget>(localController, widgetClass);
		}

		widgetInstance->AddToViewport();
		widgetInstance->SetVisibility(ESlateVisibility::Visible);

		UWidget* inventoryWidget = widgetInstance->GetWidgetFromName(FName("WBP_Inventory"));
		if (UInventoryWidget* inventory = Cast<UInventoryWidget>(inventoryWidget))
		{
			inventory->InitializeWidget(this, 40);
		}

		return;
	}

	if (widgetInstance == nullptr)
	{
		return;
	}
	
	// Viewpoint에서 UI 제거
	widgetInstance->SetVisibility(ESlateVisibility::Collapsed);
	widgetInstance->RemoveFromParent();
}
