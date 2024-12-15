// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyWidget.h"
#include "Character/EnemyPawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

void UEnemyWidget::NativeConstruct() {
	Super::NativeConstruct();
}

bool UEnemyWidget::Initialize() {
	bool bSuccess = Super::Initialize();

	if (bSuccess) {
		enemyHPBar->PercentDelegate.BindUFunction(this, "GetEnemyHPPercent");
		//enemyHPBar->VisibilityDelegate.BindUFunction(this, "GetEnemyVisibility");
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("fatal"), true, false, FColor::White, 5.0f, TEXT("None"));
	}

	return bSuccess;
}

void UEnemyWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::Tick(MyGeometry, InDeltaTime);

	enemyHPBar->SetVisibility(GetEnemyVisibility());
}

float UEnemyWidget::GetEnemyHPPercent() {

	// TODO: ƒƒbƒN‚µ‚Ä‚¢‚È‚¢Žž‚É•\Ž¦‚ðÁ‚·
	if (!pPlayer->GetIsCameraLock()) return 0.0f;

	if (!pPlayer) {
		GetPlayerPawn();
		return 0.0f;
	}

	if (!pEnemy) {
		GetEnemyPawn();
		return 0.0f;
	}

	return pEnemy->GetHPVal() / pEnemy->GetMaxPowerVal();

}

void UEnemyWidget::GetEnemyPawn() {
	if (!pPlayer) return;

	pEnemy = pPlayer->GetLockingEnemy();
}

ESlateVisibility UEnemyWidget::GetEnemyVisibility() {
	if (pPlayer->GetIsCameraLock()) return ESlateVisibility::Visible;

	return ESlateVisibility::Collapsed;
}