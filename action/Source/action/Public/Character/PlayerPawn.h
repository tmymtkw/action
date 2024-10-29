// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayerPawn.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class ACTION_API APlayerPawn : public ABaseCharacterPawn
{
	GENERATED_BODY()
	
public:
	APlayerPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UCapsuleComponent> pCapsule;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> pMesh;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UCameraComponent> pCamera;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USpringArmComponent> pSpringArm;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> pMappingContext;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pMoveInput;

	FVector2D temp;
	void SetMoveInput(const FInputActionValue& val);
};
