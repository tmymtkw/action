// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyPawn.h"

AEnemyPawn::AEnemyPawn() {
	PrimaryActorTick.bCanEverTick = false;

	// “–‚½‚è”»’è
	pBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pBody->SetCapsuleSize(30.0f, 90.0f);
	pBody->SetHiddenInGame(false);
	pBody->SetSimulatePhysics(false);
	pBody->SetCollisionProfileName("CharacterCollision");
	// TODO: Set Root Component
	pBody->SetupAttachment(RootComponent);

	TObjectPtr<USkeletalMesh> mesh =
		LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny_Simple.SKM_Manny_Simple'"));

	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	pMesh->SetSkeletalMesh(mesh);
	pMesh->SetupAttachment(pBody);
	pMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

}

void AEnemyPawn::BeginPlay() {
	fMaxPower = 100.0f;
	fHP = fMaxPower;
}

void AEnemyPawn::Tick(float DeltaTime) {

}

float AEnemyPawn::GetHPVal() { return fHP; }

float AEnemyPawn::GetMaxHPVal() { return fMaxPower; }