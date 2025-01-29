// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "PlayerActor.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class UCameraComponent;
class USpringArmComponent;
/**
 * 
 */
UCLASS()
class ACTION_API APlayerActor : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	APlayerActor();

	virtual void Tick(float DeltaTime) override;

private:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCapsuleComponent> pCapsule;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> pMesh;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> pCamera;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> pSpringArm;
};
