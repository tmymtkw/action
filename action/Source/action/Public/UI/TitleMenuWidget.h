// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleMenuWidget.generated.h"


class UButton;
/**
 * 
 */
UCLASS()
class ACTION_API UTitleMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> buttonPlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> buttonQuit;

	UFUNCTION()
	void PlayGame();

	UFUNCTION()
	void QuitGame();
};
