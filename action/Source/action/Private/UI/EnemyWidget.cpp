// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EnemyWidget.h"
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

	ESlateVisibility bEnemyVisible = GetEnemyVisibility();

	if (bEnemyVisible != enemyHPBar->GetVisibility()) {
		enemyHPBar->SetVisibility(bEnemyVisible);
		lockonCursor->SetVisibility(bEnemyVisible);
	}

	if (bEnemyVisible != ESlateVisibility::Visible) return;

	UpdateLockonCursor();
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

void UEnemyWidget::UpdateLockonCursor() {
	if (!playerController) {
		GetPlayerController();
		return;
	}

	if (!pEnemy) return;

	FVector2D screenLocation;

	playerController->ProjectWorldLocationToScreen(pEnemy->GetActorLocation(), screenLocation, true);
	lockonCursor->SetRenderTranslation(screenLocation);
}

void UEnemyWidget::GetEnemyPawn() {
	if (!pPlayer) return;

	pEnemy = pPlayer->GetLockingEnemy();
}

void UEnemyWidget::GetPlayerController() {
	playerController = UGameplayStatics::GetPlayerController(this, 0);
}

ESlateVisibility UEnemyWidget::GetEnemyVisibility() {
	if (pPlayer->GetIsCameraLock()) return ESlateVisibility::Visible;

	return ESlateVisibility::Collapsed;
}