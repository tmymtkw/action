// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/ResultWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AInGameHUD::BeginPlay() {
	FString statusWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Player/BPW_Status.BPW_Status_C'");
	FString debugWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Debug/BPW_PlayerStatusDebug.BPW_PlayerStatusDebug_C'");
	FString gameoverWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Levels/BPW_GameOver.BPW_GameOver_C'");
	FString resultWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Levels/BPW_Result.BPW_Result_C'");
	//FString cursorWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Player/BPW_LockonCursor.BPW_LockonCursor_C'");
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AddWidgetToViewport(statusWidgetPath, 0, playerController);
	AddWidgetToViewport(debugWidgetPath, 1, playerController);
	//AddWidgetToViewport(cursorWidgetPath, 2, playerController);

	playerController->SetInputMode(FInputModeGameOnly());
	playerController->SetShowMouseCursor(false);

	// gameover widget
	//TSubclassOf<UUserWidget> gameoverWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*gameoverWidgetPath)).LoadSynchronous();

	//if (!gameoverWidgetClass || !playerController) {
	//	UKismetSystemLibrary::PrintString(this, TEXT("Failed to add gameover widget"), true, false, FColor::Green, 5.0f, TEXT("None"));
	//	return;
	//}

	//gameoverWidget = UWidgetBlueprintLibrary::Create(GetWorld(), gameoverWidgetClass, playerController);
	//gameoverWidget->SetVisibility(ESlateVisibility::Collapsed);
	//gameoverWidget->AddToViewport(3);
	//UKismetSystemLibrary::PrintString(this, TEXT("succeded to add gameover widget"), true, false, FColor::Green, 5.0f, TEXT("None"));

	TSubclassOf<UUserWidget> resultWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*resultWidgetPath)).LoadSynchronous();

	if (!resultWidgetClass || !playerController) {
		UKismetSystemLibrary::PrintString(this, TEXT("Failed to add result widget"), true, false, FColor::Green, 5.0f, TEXT("None"));
		return;
	}

	resultWidget = UWidgetBlueprintLibrary::Create(GetWorld(), resultWidgetClass, playerController);
	resultWidget->SetVisibility(ESlateVisibility::Collapsed);
	resultWidget->AddToViewport(3);
	UKismetSystemLibrary::PrintString(this, TEXT("succeded to add result widget"), true, false, FColor::Green, 5.0f, TEXT("None"));
}

void AInGameHUD::AddWidgetToViewport(FString path, int32 index, APlayerController* controller) {
	TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

	if (!widgetClass || !controller) {
		UKismetSystemLibrary::PrintString(this, TEXT("Failed to add widget"), true, false, FColor::Green, 5.0f, TEXT("None"));
		return;
	}

	TObjectPtr<UUserWidget> newWidget = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, controller);

	newWidget->AddToViewport(index);
}

void AInGameHUD::DisplayGameOver() {
	gameoverWidget->SetVisibility(ESlateVisibility::Visible);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(playerController, gameoverWidget, EMouseLockMode::DoNotLock, false);
	playerController->SetShowMouseCursor(true);
}

void AInGameHUD::DisplayResult(bool isClear) {
	castedResultWidget = Cast<UResultWidget>(resultWidget);
	castedResultWidget->SetResult(isClear);
	resultWidget->SetVisibility(ESlateVisibility::Visible);

	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(playerController, resultWidget, EMouseLockMode::DoNotLock, false);
	playerController->SetShowMouseCursor(true);
}