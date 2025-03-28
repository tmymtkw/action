// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeBaseInGame.h"
#include "Character/PlayerPawn.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UI/InGameHUD.h"

AGameModeBaseInGame::AGameModeBaseInGame() {
	DefaultPawnClass = APlayerPawn::StaticClass();
	HUDClass = AInGameHUD::StaticClass();
}

void AGameModeBaseInGame::BeginPlay() {
	Super::BeginPlay();

	const APlayerStart* PlayerStart =
		Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));
	//UKismetSystemLibrary::PrintString(this, PlayerStart->GetActorLocation().ToString(), true, false, FColor::Red, 5.0f, TEXT("None"));

	vSpawnTransform = PlayerStart->GetActorTransform();
}

void AGameModeBaseInGame::ActivateGameOver() {
	const TObjectPtr<APlayerController> playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TObjectPtr<AInGameHUD> hud = Cast<AInGameHUD>(playerController->GetHUD());
	if (hud) {
		hud->DisplayGameOver();
	}
}

void AGameModeBaseInGame::ActivateResult(bool isClear) {
	FTimerManager& timerManager = GetWorldTimerManager();
	FTimerHandle timerHandle;

	UKismetSystemLibrary::PrintString(this, TEXT("make gamespeed slow"), true, false, FColor::Green, 5.0f, TEXT("None"));
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2);
	timerManager.SetTimer(timerHandle, this, &AGameModeBaseInGame::ResetGameSpeed, 0.4f, false, 0.4f);

	const TObjectPtr<APlayerController> playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	TObjectPtr<AInGameHUD> hud = Cast<AInGameHUD>(playerController->GetHUD());

	if (hud) {
		hud->DisplayResult(isClear);
	}
}

void AGameModeBaseInGame::Respawn() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TObjectPtr<APlayerPawn> Player = GetWorld()->SpawnActor<APlayerPawn>(APlayerPawn::StaticClass(), vSpawnTransform, SpawnParams);

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//PlayerController->Possess(Player);
}

void AGameModeBaseInGame::ResetGameSpeed() {
	UKismetSystemLibrary::PrintString(this, TEXT("reset gamespeed"), true, false, FColor::Green, 5.0f, TEXT("None"));

	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
}

//void AGameModeBaseInGame::KillPlayer(TObjectPtr<APlayerPawn2> Player) {
//	Player->Destroy();
//
//	Respawn();
//}