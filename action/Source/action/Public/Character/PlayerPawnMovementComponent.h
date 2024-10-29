// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "PlayerPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class ACTION_API UPlayerPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:
	UPlayerPawnMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	
private:

};
