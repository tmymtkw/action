// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BaseCharacterPawn.h"

// Sets default values
ABaseCharacterPawn::ABaseCharacterPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ABaseCharacterPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseCharacterPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseCharacterPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

