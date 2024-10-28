// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameModeBaseInGame.generated.h"

class APlayerPawn;
/**
 * 
 */
UCLASS()
class ACTION_API AGameModeBaseInGame : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameModeBaseInGame();

protected:
	virtual void BeginPlay() override;

public:
	FTransform vSpawnTransform;

	void KillPlayer(TObjectPtr<APlayerPawn> Player);

private:
	void Respawn();
};
