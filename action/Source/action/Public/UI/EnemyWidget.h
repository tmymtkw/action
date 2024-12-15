// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/PlayerStatusWidget.h"
#include "Components/ProgressBar.h"
#include "EnemyWidget.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UEnemyWidget : public UPlayerStatusWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<ABaseCharacterPawn> pEnemy;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> enemyHPBar;

	UFUNCTION()
	float GetEnemyHPPercent();

	void GetEnemyPawn();

	ESlateVisibility GetEnemyVisibility();
};
