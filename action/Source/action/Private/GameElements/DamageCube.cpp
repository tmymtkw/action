// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/DamageCube.h"

ADamageCube::ADamageCube() {
	PrimaryActorTick.bCanEverTick = false;

	pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	pBox->SetHiddenInGame(false);
	pBox->SetBoxExtent(FVector(100.0f, 50.0f, 50.0f), true);
	pBox->SetupAttachment(RootComponent);
	pBox->SetCollisionProfileName("AttackCollision");

	//AddActorTag(FName("Enemy"));
}

void ADamageCube::BeginPlay() {
	Super::BeginPlay();

	SetLifeSpan(1.0f);
}