// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/EnemyAttackCube.h"
#include "Components/SphereComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"

AEnemyAttackCube::AEnemyAttackCube() {
	PrimaryActorTick.bCanEverTick = true;

	pCollision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	pCollision->ShapeColor = FColor::Red;
	pCollision->SetHiddenInGame(true);
	pCollision->SetSphereRadius(50.0f);
	pCollision->SetupAttachment(RootComponent);
	pCollision->SetCollisionProfileName("AttackCollision");

	pEffect = LoadObject<UNiagaraSystem>(nullptr, TEXT("/Script/Niagara.NiagaraSystem'/Game/Niagara/NS_EnemyBullet.NS_EnemyBullet'"));

	pEffectComp = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	pEffectComp->SetupAttachment(RootComponent);
	pEffectComp->SetAsset(pEffect, false);
}

void AEnemyAttackCube::BeginPlay() {
	Super::BeginPlay();

	speed = 700.0f;

	pEffectComp->Activate(true);
}

void AEnemyAttackCube::Tick(float deltaTime) {
	pCollision->AddRelativeLocation(this->GetActorForwardVector() * speed * deltaTime);
	pEffectComp->AddRelativeLocation(this->GetActorForwardVector() * speed * deltaTime);

	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), pEffect, pCollision->GetComponentLocation(), pCollision->GetComponentRotation(),
	//																							 FVector(1.0f), true, true, ENCPoolMethod::AutoRelease, true);
}