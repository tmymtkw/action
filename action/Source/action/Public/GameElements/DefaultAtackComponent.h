// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework\Actor.h"
#include "DefaultAtackComponent.generated.h"

UCLASS()
class ACTION_API ADefaultAtackComponent : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADefaultAtackComponent();

	virtual void Tick(float DeltaTime);

protected:
	//// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
