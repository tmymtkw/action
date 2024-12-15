// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AInGameHUD::BeginPlay() {
	FString statusWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Player/BPW_Status.BPW_Status_C'");
	FString debugWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Debug/BPW_PlayerStatusDebug.BPW_PlayerStatusDebug_C'");
	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AddWidgetToViewport(statusWidgetPath, 0, playerController);
	AddWidgetToViewport(debugWidgetPath, 1, playerController);
}

void AInGameHUD::AddWidgetToViewport(FString path, int32 index, APlayerController* controller) {
	TSubclassOf<UUserWidget> widgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*path)).LoadSynchronous();

	if (!widgetClass || !controller) {
		UKismetSystemLibrary::PrintString(this, TEXT("Failed to add widget"), true, false, FColor::Red, 5.0f, TEXT("None"));
		return;
	}

	TObjectPtr<UUserWidget> newWidget = UWidgetBlueprintLibrary::Create(GetWorld(), widgetClass, controller);

	newWidget->AddToViewport(index);
}