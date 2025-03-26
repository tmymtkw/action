// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

class UUserWidget;
class UResultWidget;
/**
 * 
 */
UCLASS()
class ACTION_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	void DisplayGameOver();

	void DisplayResult(bool isClear);

protected:
	virtual void BeginPlay() override;

private:
	TObjectPtr<UUserWidget> gameoverWidget;

	TObjectPtr<UUserWidget> resultWidget;
	TObjectPtr<UResultWidget> castedResultWidget;

	void AddWidgetToViewport(FString path, int32 index, APlayerController* controller);

	void Reveal();
};
