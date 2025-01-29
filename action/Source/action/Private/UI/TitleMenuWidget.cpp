// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleMenuWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"

void UTitleMenuWidget::NativeConstruct() {
	Super::NativeConstruct();

	buttonPlay->OnClicked.AddUniqueDynamic(this, &UTitleMenuWidget::PlayGame);

	buttonQuit->OnClicked.AddUniqueDynamic(this, &UTitleMenuWidget::QuitGame);
}

void UTitleMenuWidget::PlayGame() {
	
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGameDev"));
}

void UTitleMenuWidget::QuitGame() {
	if (TObjectPtr<APlayerController> pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		UKismetSystemLibrary::QuitGame(GetWorld(), pPlayerController, EQuitPreference::Quit, false);
	}
}