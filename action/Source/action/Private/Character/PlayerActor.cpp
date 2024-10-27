// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"

APlayerActor::APlayerActor() {
	PrimaryActorTick.bCanEverTick = false;

	pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pCapsule->SetCapsuleSize(30.0f, 90.0f);
	pCapsule->SetHiddenInGame(false);
	pCapsule->SetSimulatePhysics(false);
	// TODO: Set Root Component
	pCapsule->SetupAttachment(RootComponent);

	TObjectPtr<USkeletalMesh> mesh =
		LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	pMesh->SetSkeletalMesh(mesh);
	pMesh->SetupAttachment(pCapsule);
	pMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));
}

void APlayerActor::BeginPlay() {
	Super::BeginPlay();

}

void APlayerActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

}
