// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CursorWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UCursorWidget::NativeConstruct() {
	Super::NativeConstruct();

	UKismetSystemLibrary::PrintString(this, TEXT("cursor tick is created"), true, false, FColor::White, 5.0f, TEXT("None"));
}

bool UCursorWidget::Initialize() {
	bool bSuccess = Super::Initialize();

	GetPlayerPawn();

	return bSuccess;
}

void UCursorWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	//Super::NativeTick(const FGeometry & MyGeometry, float InDeltaTime);

	UKismetSystemLibrary::PrintString(this, TEXT("cursor tick is called"), true, false, FColor::White, InDeltaTime, TEXT("None"));
}

void UCursorWidget::GetPlayerPawn() {
	pPlayer = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}