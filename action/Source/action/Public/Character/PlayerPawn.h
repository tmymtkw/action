// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterPawn.h"
//#include "InputMappingContext.h"
//#include "InputAction.h"
#include "InputActionValue.h"
#include "PlayerStatus.h"
#include "PlayerPawn.generated.h"

//class USkeletalMeshComponent;
//class UCapsuleComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UPlayerPawnMovementComponent;
class UInputMappingContext;
class UInputAction;
class UPlayerAnimInstance;
class UCursorWidget;
class UNiagaraSystem;
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

	//float GetHPVal();

	float GetAPVal();

	float GetSPVal();

	FRotator GetPlayerRotation();

	TObjectPtr<ABaseCharacterPawn> GetLockingEnemy();

	bool GetIsCameraLock();

	UFUNCTION(BlueprintCallable, Category = "CPPFunction|PlayerPawn")
	bool GetIsPowerAttack();

	void WeakAttack();

	void PowerAttack();

	void RecoverFromAttack(float val) override;

	TObjectPtr<UCursorWidget> cursorWidget;

protected:
	virtual void BeginPlay() override;

private:
	PlayerStatus params;

	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//TObjectPtr<UCapsuleComponent> pCapsule;
	//UPROPERTY(VisibleAnywhere, Category = "Component")
	//TObjectPtr<USkeletalMeshComponent> pMesh;
	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USphereComponent> pAvoid;
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
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pWeakAttackInput;
	UPROPERTY(VisibleAnywhere, Category = "Input")
	TObjectPtr<UInputAction> pPowerAttackInput;

	UPROPERTY()
	FRotator rLookInput;

	UPROPERTY()
	TObjectPtr<UPlayerAnimInstance> animInstance;

	//float HP;

	float AP;

	float SP;

	bool bAvoidOverlap;

	bool bBlink;
	float fBlinkTime;

	bool bSprint;

	bool bHeal;

	bool bCameraLock;

	bool bAttack;

	//bool bPowerAttack;

	TObjectPtr<ABaseCharacterPawn> lockingEnemy;

	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjects;

	//TObjectPtr<UNiagaraComponent> attackEffect;
	TObjectPtr<UNiagaraSystem> niagaraAttackSystem;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnAvoidOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnAvoidOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void SetMoveInput(const FInputActionValue& val);
	void SetLookInput(const FInputActionValue& val);
	void SetHealInput(const FInputActionValue& val);
	void SetBlinkInput();
	void SetSprintInput();
	void SetPowerAttackInput();

	void UpdateCameraAngle();

	void HealHP(const float& DeltaTime);

	void UpdateCameraLock();

	void SetCameraLag(float val);

	void SetCameraRotationLag(float val);
};
