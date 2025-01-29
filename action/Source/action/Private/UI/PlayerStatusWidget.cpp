// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PlayerStatusWidget.h"
#include "Components/ProgressBar.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UPlayerStatusWidget::NativeConstruct() {
	Super::NativeConstruct();
}

bool UPlayerStatusWidget::Initialize() {
	bool bSuccess = Super::Initialize();

	GetPlayerPawn();

	if (bSuccess) {
		playerHPBar->PercentDelegate.BindUFunction(this, "GetPlayerHPPercent");
		playerAPBar->PercentDelegate.BindUFunction(this, "GetPlayerAPPercent");
		playerSPBar->PercentDelegate.BindUFunction(this, "GetPlayerSPPercent");
		UKismetSystemLibrary::PrintString(this, TEXT("success"), true, false, FColor::White, 5.0f, TEXT("None"));
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("fatal"), true, false, FColor::White, 5.0f, TEXT("None"));
	}

	return bSuccess;
}

float UPlayerStatusWidget::GetPlayerHPPercent() {
	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	return pPlayer->GetHPVal() / pPlayer->GetMaxPowerVal();
}

float UPlayerStatusWidget::GetPlayerAPPercent() {
	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	return (pPlayer->GetHPVal() + pPlayer->GetAPVal()) / pPlayer->GetMaxPowerVal();
}

float UPlayerStatusWidget::GetPlayerSPPercent() {
	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	return pPlayer->GetSPVal() / pPlayer->GetMaxPowerVal();

}

void UPlayerStatusWidget::GetPlayerPawn() {
	pPlayer = Cast<APlayerPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}