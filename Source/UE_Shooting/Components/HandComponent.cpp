// Fill out your copyright notice in the Description page of Project Settings.


#include "HandComponent.h"
#include "EnhancedInputComponent.h"
#include "UE_Shooting/Item/ItemBase.h"

// Sets default values for this component's properties
UHandComponent::UHandComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHandComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHandComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHandComponent::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		//enhancedInputComponent->BindAction();
	}
}

void UHandComponent::TakeItem(FPrimaryAssetId itemDataID)
{
	actorInHand->AssignItemData(itemDataID);
}
