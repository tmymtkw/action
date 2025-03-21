// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterPawn.h"
#include "TrainingMachinePawn.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API ATrainingMachinePawn : public ABaseCharacterPawn
{
	GENERATED_BODY()
	
public:
	ATrainingMachinePawn();

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

private:
	TObjectPtr<UStaticMeshComponent> pStaticBody;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	TObjectPtr<ABaseCharacterPawn> pPlayer;

	float fInterval;
	float fTime;
	int fCnt;

	void GetPlayerPawn();
};
