// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameOverWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"


void UGameOverWidget::NativeConstruct() {
	Super::NativeConstruct();

	retryButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::RetryGame);

	quitButton->OnClicked.AddUniqueDynamic(this, &UGameOverWidget::QuitGame);

}

void UGameOverWidget::RetryGame() {
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGameDev"));

}

void UGameOverWidget::QuitGame() {
	if (TObjectPtr<APlayerController> pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		UKismetSystemLibrary::QuitGame(GetWorld(), pPlayerController, EQuitPreference::Quit, false);
	}

}