// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "BaseCharacterPawn.generated.h"

UCLASS()
class ACTION_API ABaseCharacterPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseCharacterPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<UCapsuleComponent> pBody;

	UPROPERTY(VisibleAnywhere, Category = "Component")
	TObjectPtr<USkeletalMeshComponent> pMesh;

	float fHP;

	float fMaxPower;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	float GetHPVal();

	float GetMaxPowerVal();

	virtual void RecoverFromAttack(float val);

private:
	bool bPlayer;

};
