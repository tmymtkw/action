// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStatusWidget.generated.h"

class APlayerPawn;
class UProgressBar;
/**
 * 
 */
UCLASS()
class ACTION_API UPlayerStatusWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

private:
	UPROPERTY()
	TObjectPtr<APlayerPawn> pPlayer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> HPBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> APBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> SPBar;
	
	UFUNCTION()
	float GetHPPercent();
	UFUNCTION()
	float GetAPPercent();
	UFUNCTION()
	float GetSPPercent();

	void GetPlayerPawn();
};
