// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterPawn.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayerStatus.h"
#include "PlayerPawn.generated.h"

//class USkeletalMeshComponent;
//class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
class UPlayerPawnMovementComponent;
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

	float GetHPVal();

	float GetAPVal();

	float GetSPVal();

	float GetMaxPower();

protected:
	virtual void BeginPlay() override;

private:
	PlayerStatus params;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//TObjectPtr<UCapsuleComponent> pCapsule;
	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//TObjectPtr<USkeletalMeshComponent> pMesh;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UCameraComponent> pCamera;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USpringArmComponent> pSpringArm;
	UPROPERTY(VisibleAnyWhere, Category = "Component")
	TObjectPtr<UPlayerPawnMovementComponent> pPawnMove;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> pMappingContext;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pMoveInput;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pLookInput;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pBlinkInput;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pSprintInput;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pHealInput;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pCameraLockInput;

	UPROPERTY()
	FRotator rLookInput;

	float HP;

	float AP;

	float SP;

	bool bBlink;
	float fBlinkTime;

	bool bSprint;

	bool bHeal;

	bool bCameraLock;

	TObjectPtr<AActor> lockingEnemy;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjects;

	void SetMoveInput(const FInputActionValue& val);
	void SetLookInput(const FInputActionValue& val);
	void SetHealInput(const FInputActionValue& val);
	void SetBlinkInput();
	void SetSprintInput();

	void UpdateCameraAngle();

	void HealHP(const float& DeltaTime);

	void UpdateCameraLock();
};
