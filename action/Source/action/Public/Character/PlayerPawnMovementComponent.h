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

	void SetRotatedComponent(TObjectPtr<USceneComponent> newRotatedComponent);

	void SetInput(const FVector2D& val);

	void UpdatePawnMovement(float DeltaTime, const FRotator& cameraAngle);

	void UpdateComponentRotation(float DeltaTime);

protected:
	
private:
	FVector vInput;

	FVector vRotatedInput;

	TObjectPtr<USceneComponent> rotatedComponent;

	void GetRotatedInput(const FRotator& cameraAngle);

	FVector GetVelocity(float DeltaTime);
};
