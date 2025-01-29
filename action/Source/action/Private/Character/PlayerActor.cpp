// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerActor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetSystemLibrary.h"

APlayerActor::APlayerActor() {
	PrimaryActorTick.bCanEverTick = true;

	// 当たり判定
	pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pCapsule->SetCapsuleSize(30.0f, 90.0f);
	pCapsule->SetHiddenInGame(false);
	pCapsule->SetSimulatePhysics(false);
	// TODO: Set Root Component
	pCapsule->SetupAttachment(RootComponent);

	// メッシュ
	TObjectPtr<USkeletalMesh> mesh =
		LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	pMesh->SetSkeletalMesh(mesh);
	pMesh->SetupAttachment(pCapsule);
	pMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// カメラ関連
	// カメラの移動範囲
	pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	pSpringArm->SetupAttachment(pCapsule);
	pSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 80.0f), FRotator(-10.0f, 0.0f, 0.0f));
	pSpringArm->TargetArmLength = 300.f;

	// カメラ
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	pCamera->SetupAttachment(pSpringArm);

}

void APlayerActor::BeginPlay() {
	Super::BeginPlay();

}

void APlayerActor::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	UKismetSystemLibrary::PrintString(this, this->GetActorLocation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, pCapsule->GetComponentLocation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
}
