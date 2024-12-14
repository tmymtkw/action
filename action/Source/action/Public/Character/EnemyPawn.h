// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterPawn.h"
#include "EnemyPawn.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API AEnemyPawn : public ABaseCharacterPawn
{
	GENERATED_BODY()
	
public:
	AEnemyPawn();

protected:
	virtual void Tick(float DeltaTime) override;



private:

};
