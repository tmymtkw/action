// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerPawn.h"
#include "PlayerStatusWidget.generated.h"

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

	UPROPERTY()
	TObjectPtr<APlayerPawn> pPlayer;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> playerHPBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> playerAPBar;
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UProgressBar> playerSPBar;

	UFUNCTION()
	float GetPlayerHPPercent();
	UFUNCTION()
	float GetPlayerAPPercent();
	UFUNCTION()
	float GetPlayerSPPercent();

	void GetPlayerPawn();

private:
};
