// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TrainingMachinePawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameElements/DamageCube.h"

ATrainingMachinePawn::ATrainingMachinePawn() {
	PrimaryActorTick.bCanEverTick = true;

	pBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pBody->SetCapsuleSize(50.0f, 120.0f);
	pBody->SetHiddenInGame(false);
	pBody->SetSimulatePhysics(false);
	pBody->SetCollisionProfileName("CharacterCollision");
	pBody->OnComponentBeginOverlap.AddDynamic(this, &ATrainingMachinePawn::OnOverlapBegin);
	pBody->SetupAttachment(RootComponent);

	TObjectPtr<UStaticMesh> m = LoadObject<UStaticMesh>(nullptr,
																											TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	pStaticBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	pStaticBody->SetStaticMesh(m);
	pStaticBody->SetupAttachment(pBody);

}

void ATrainingMachinePawn::BeginPlay() {
	Super::BeginPlay();

	fMaxPower = 100.0f;
	fHP = fMaxPower;
	fInterval = 3.0f;
	fTime = 0.0f;
}

void ATrainingMachinePawn::Tick(float deltaTime) {
	Super::Tick(deltaTime);

	fTime += deltaTime;
	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(fTime), true, false, FColor::Red, deltaTime, TEXT("None"));

	if (fTime < fInterval) return;

	if (!pPlayer) {
		GetPlayerPawn();
		return;
	}

	fTime = 0.0f;
	UKismetSystemLibrary::PrintString(this, TEXT("Attack Interval"), true, false, FColor::Red, 5.0f, TEXT("None"));

	// 攻撃アクタのスポーン
	FVector Location = pBody->GetComponentLocation();
	FRotator Rotation = pBody->GetComponentRotation() + FRotator(0.0f, 90.0f, 0.0f);

	TObjectPtr<UWorld> World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	TObjectPtr<ADamageCube> attack = World->SpawnActor<ADamageCube>(Location, Rotation, SpawnParams);
	attack->AddActorTag(FName("Enemy"));
	// 速度
	attack->speed = 500.0f;
	attack->SetLifeSpan(3.0f);
	attack->SetDamageValue(10.0f);
}

void ATrainingMachinePawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	UKismetSystemLibrary::PrintString(this, TEXT("overlap occured"), true, false, FColor::Red, 5.0f, TEXT("None"));

	if (!OtherActor->ActorHasTag(FName("Player"))) return;

	TObjectPtr<ADamageCube> actor = Cast<ADamageCube>(OtherActor);
	fHP = FMathf::Max(0.0f, fHP - actor->GetDamageValue());
	pPlayer->RecoverFromAttack(actor->GetDamageValue());

	UKismetSystemLibrary::PrintString(this, TEXT("Enemy damaged"), true, false, FColor::Red, 5.0f, TEXT("None"));
}

void ATrainingMachinePawn::GetPlayerPawn() {
	pPlayer = Cast<ABaseCharacterPawn>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
}