// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/InGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/UserWidget.h"

void AInGameHUD::BeginPlay() {
	FString StatusWidgetPath = TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/Player/BPW_Status.BPW_Status_C'");
	TSubclassOf<UUserWidget> StatusWidgetClass = TSoftClassPtr<UUserWidget>(FSoftObjectPath(*StatusWidgetPath)).LoadSynchronous();
	if (!StatusWidgetClass) UKismetSystemLibrary::PrintString(this, TEXT("Fatal"), true, false, FColor::Red, 5.0f, TEXT("None"));
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (StatusWidgetClass && PlayerController) {
		TObjectPtr<UUserWidget> StatusWidget = UWidgetBlueprintLibrary::Create(GetWorld(), StatusWidgetClass, PlayerController);
		//StatusWidget->SetVisibility(ESlateVisibility::Collapsed);
		StatusWidget->AddToViewport(0);
	}
}