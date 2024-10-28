// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacterPawn.h"
#include "PlayerPawn.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> pCapsule;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> pMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> pCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> pSpringArm;
};
