// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void ATitleMenuHUD::BeginPlay() {
	FString path = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Levels/BPW_Title.BPW_Title_C'");
	TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

	TObjectPtr<APlayerController> playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (!widgetClass || !playerController) {
		UKismetSystemLibrary::PrintString(this, TEXT("Failed to add widget"), true, false, FColor::Red, 5.0f, TEXT("None"));
		return;
	}

	TObjectPtr<UUserWidget> widget = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, playerController);

	widget->AddToViewport(0);

	UWidgetBlueprintLibrary::SetInputMode_GameAndUIEx(playerController, widget, EMouseLockMode::DoNotLock, true, false);
	playerController->SetShowMouseCursor(true);
}