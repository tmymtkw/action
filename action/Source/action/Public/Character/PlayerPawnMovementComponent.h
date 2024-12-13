// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MovementStatus.h"
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

	void SetSprintInput();

	void SetBlinkDirection(const FRotator& cameraAngle);

	void UpdatePawnMovement(const float& DeltaTime, bool isBlink, const FRotator& cameraAngle);

	void UpdateComponentRotation(const float& DeltaTime);

	FString GetInputValue();

protected:
	
private:
	FVector vInput;

	FVector vRotatedInput;

	FVector vBlinkDir;

	TObjectPtr<USceneComponent> rotatedComponent;

	void GetIsGround(FHitResult& hitResult);

	void GetRotatedInput(const FRotator& cameraAngle);

	FVector GetVelocity(const float& DeltaTime, bool& isBlink);

	bool bIsGround;

	bool bSprint;

	float fJumpSpeed;

	FVector_NetQuantizeNormal hitGround;

	MovementStatus params;
};
