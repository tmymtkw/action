// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameElements/DefaultAtackActor.h"
#include "DamageCube.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class ACTION_API ADamageCube : public ADefaultAtackActor
{
	GENERATED_BODY()
	
public:
	ADamageCube();

	void SetCollisionSpeed(float f);

	void SetCollisionSize(FVector size);


protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

private:
	UPROPERTY()
	TObjectPtr<UBoxComponent> pBox;

	UPROPERTY()
	float speed;
	//TObjectPtr<UStaticMeshComponent> pMesh;
};
