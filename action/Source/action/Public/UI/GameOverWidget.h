// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameOverWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class ACTION_API UGameOverWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> retryButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> quitButton;

	UFUNCTION()
	void RetryGame();

	UFUNCTION()
	void QuitGame();
};
