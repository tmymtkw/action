// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/DebugWidget.h"
#include "Character/PlayerPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UDebugWidget::NativeConstruct() {
	Super::NativeConstruct();
}

bool UDebugWidget::Initialize() {
	bool bSuccess = Super::Initialize();

	GetPlayerPawn();

	if (bSuccess) {
		locationInformation->TextDelegate.BindUFunction(this, "GetLocationInformation");
		rotationInformation->TextDelegate.BindUFunction(this, "GetRotationInformation");
		hpInformation->TextDelegate.BindUFunction(this, "GetHPInformation");
		apInformation->TextDelegate.BindUFunction(this, "GetAPInformation");
		spInformation->TextDelegate.BindUFunction(this, "GetSPInformation");
	}

	return bSuccess;
}

FText UDebugWidget::GetLocationInformation() {
	if (!pPlayer) {
		GetPlayerPawn();
		return FText::FromString("-.- -.- -.-");
	}

	return FText::FromString(pPlayer->GetActorLocation().ToString());
}

FText UDebugWidget::GetRotationInformation() {
	if (!pPlayer) return FText::FromString("-.- -.- -.-");

	return FText::FromString(pPlayer->GetPlayerRotation().ToString());
}

FText UDebugWidget::GetHPInformation() {
	if (!pPlayer) return FText::FromName(zero);

	return FText::FromString(FString::SanitizeFloat(pPlayer->GetHPVal()));
}

FText UDebugWidget::GetAPInformation() {
	if (!pPlayer) return FText::FromName(zero);

	return FText::FromString(FString::SanitizeFloat(pPlayer->GetAPVal()));
}

FText UDebugWidget::GetSPInformation() {
	if (!pPlayer) return FText::FromName(zero);

	return FText::FromString(FString::SanitizeFloat(pPlayer->GetSPVal()));
}

void UDebugWidget::GetPlayerPawn() {
	pPlayer = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}