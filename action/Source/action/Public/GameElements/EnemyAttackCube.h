// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/DefaultAtackActor.h"
#include "EnemyAttackCube.generated.h"

class USphereComponent;
class UNiagaraSystem;
class UNiagaraComponent;
/**
 * 
 */
UCLASS()
class ACTION_API AEnemyAttackCube : public ADefaultAtackActor
{
	GENERATED_BODY()
	
	TObjectPtr<USphereComponent> pCollision;

	TObjectPtr<UNiagaraSystem> pEffect;

	TObjectPtr<UNiagaraComponent> pEffectComp;

	float speed;

public:
	AEnemyAttackCube();

protected:
	void BeginPlay() override;

	void Tick(float deltaTime) override;
};
