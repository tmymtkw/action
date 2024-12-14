// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Character/PlayerPawn.h"

void UPlayerStatusWidget::NativeConstruct() {
	Super::NativeConstruct();
}

bool UPlayerStatusWidget::Initialize() {
	bool bSuccess = Super::Initialize();

	GetPlayerPawn();

	if (bSuccess) {
		HPBar->PercentDelegate.BindUFunction(this, "GetHPPercent");
		APBar->PercentDelegate.BindUFunction(this, "GetAPPercent");
		SPBar->PercentDelegate.BindUFunction(this, "GetSPPercent");
	}

	return !pPlayer && bSuccess;
}

float UPlayerStatusWidget::GetHPPercent() {
	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	return pPlayer->GetHPVal() / pPlayer->GetMaxPower();
}

float UPlayerStatusWidget::GetAPPercent() {
	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	return (pPlayer->GetHPVal() + pPlayer->GetAPVal()) / pPlayer->GetMaxPower();
}

float UPlayerStatusWidget::GetSPPercent() {
	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	return pPlayer->GetSPVal() / pPlayer->GetMaxPower();

}

void UPlayerStatusWidget::GetPlayerPawn() {
	pPlayer = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}