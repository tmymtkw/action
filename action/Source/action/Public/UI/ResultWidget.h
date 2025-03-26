// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ResultWidget.generated.h"

class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class ACTION_API UResultWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetLevelName(FName name);

	void SetResult(bool isClear);
	
protected:
	void NativeConstruct() override;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> loadButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton> quitButton;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> loadLabel;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> resultText;

	UFUNCTION()
	void LoadLevel();

	UFUNCTION()
	void QuitGame();

private:
	FName levelName;
};
