// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyPawn.h"
#include "Kismet/KismetSystemLibrary.h"

AEnemyPawn::AEnemyPawn() {
	PrimaryActorTick.bCanEverTick = false;

	// “–‚½‚è”»’è
	pBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pBody->SetCapsuleSize(30.0f, 90.0f);
	pBody->SetHiddenInGame(false);
	pBody->SetSimulatePhysics(false);
	pBody->SetCollisionProfileName("CharacterCollision");
	pBody->OnComponentBeginOverlap.AddDynamic(this, &AEnemyPawn::OnOverlapBegin);
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

void AEnemyPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	fHP = FMathf::Max(0.0f, fHP - 10.0f);

	UKismetSystemLibrary::PrintString(this, TEXT("Enemy damaged"), true, false, FColor::Red, 5.0f, TEXT("None"));
}

float AEnemyPawn::GetHPVal() { return fHP; }

float AEnemyPawn::GetMaxHPVal() { return fMaxPower; }