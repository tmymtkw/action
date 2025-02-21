// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/DefaultAtackActor.h"
#include "Components/BoxComponent.h"
#include "DamageCube.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ADamageCube : public ADefaultAtackActor
{
	GENERATED_BODY()
	
public:
	ADamageCube();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<UBoxComponent> pBox;

};
