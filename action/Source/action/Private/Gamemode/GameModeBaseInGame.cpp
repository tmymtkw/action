// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/GameModeBaseInGame.h"
#include "Character/PlayerActor.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
//#include "StatusHUD.h"

AGameModeBaseInGame::AGameModeBaseInGame() {
	DefaultPawnClass = APlayerActor::StaticClass();
	//HUDClass = AStatusHUD::StaticClass();
}

void AGameModeBaseInGame::BeginPlay() {
	Super::BeginPlay();

	const APlayerStart* PlayerStart =
		Cast<APlayerStart>(UGameplayStatics::GetActorOfClass(GetWorld(), APlayerStart::StaticClass()));

	vSpawnTransform = PlayerStart->GetActorTransform();
}

void AGameModeBaseInGame::Respawn() {
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	TObjectPtr<APlayerActor> Player = GetWorld()->SpawnActor<APlayerActor>(APlayerActor::StaticClass(), vSpawnTransform, SpawnParams);

	//APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//PlayerController->Possess(Player);
}

//void AGameModeBaseInGame::KillPlayer(TObjectPtr<APlayerActor2> Player) {
//	Player->Destroy();
//
//	Respawn();
//}