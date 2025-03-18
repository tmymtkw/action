// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/DamageCube.h"

ADamageCube::ADamageCube() {
	PrimaryActorTick.bCanEverTick = true;

	pBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	pBox->ShapeColor = FColor::Red;
	pBox->SetHiddenInGame(false);
	pBox->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f), true);
	pBox->SetupAttachment(RootComponent);
	pBox->SetCollisionProfileName("AttackCollision");

	TObjectPtr<UStaticMesh> m = LoadObject<UStaticMesh>(nullptr, TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	pMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	pMesh->SetHiddenInGame(false);
	pMesh->SetStaticMesh(m);
	pMesh->SetSimulatePhysics(false);
	pMesh->SetupAttachment(pBox);
	pMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//AddActorTag(FName("Enemy"));
	this->speed = 50.0f;
	SetLifeSpan(1.0f);
}

void ADamageCube::BeginPlay() {
	Super::BeginPlay();

}

void ADamageCube::Tick(float deltaTime) {
	pBox->AddRelativeLocation(this->GetActorForwardVector() * speed * deltaTime);
}