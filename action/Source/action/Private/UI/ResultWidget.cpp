// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ResultWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/GameInstance.h"


void UResultWidget::NativeConstruct() {
	Super::NativeConstruct();

	loadButton->OnClicked.AddUniqueDynamic(this, &UResultWidget::LoadLevel);

	quitButton->OnClicked.AddUniqueDynamic(this, &UResultWidget::QuitGame);

	levelName = FName("InGameDev");

	resultText->SetText(FText::FromName(TEXT("DEBUG")));
	loadLabel->SetText(FText::FromName(TEXT("DEBUG")));
}

void UResultWidget::SetLevelName(FName name) {
	levelName = name;
}

void UResultWidget::SetResult(bool isClear) {
	if (isClear) {
		resultText->SetText(FText::FromName(TEXT("Clear!")));
		loadLabel->SetText(FText::FromName(TEXT("Main Menu")));
		this->SetLevelName(TEXT("TitleMenu"));
	}
	else {
		resultText->SetText(FText::FromName(TEXT("Failed...")));
		loadLabel->SetText(FText::FromName(TEXT("Retry")));
		this->SetLevelName(TEXT("InGameDev"));
	}
}

void UResultWidget::LoadLevel() {
	UKismetSystemLibrary::PrintString(this, levelName.ToString(), true, false, FColor::Green, 5.0f, TEXT("None"));
	UGameplayStatics::OpenLevel(GetWorld(), levelName);
}

void UResultWidget::QuitGame() {
	if (TObjectPtr<APlayerController> pPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0)) {
		UKismetSystemLibrary::QuitGame(GetWorld(), pPlayerController, EQuitPreference::Quit, false);
	}
}