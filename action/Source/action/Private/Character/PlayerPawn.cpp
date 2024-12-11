// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerPawnMovementComponent.h"


APlayerPawn::APlayerPawn() {
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
	pSpringArm->bInheritPitch = false;
	pSpringArm->bInheritYaw = false;
	pSpringArm->bInheritRoll = false;

	// カメラ
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	pCamera->SetupAttachment(pSpringArm);

	// 移動コンポーネント
	pPawnMove = CreateDefaultSubobject<UPlayerPawnMovementComponent>(TEXT("PawnMove"));
	pPawnMove->SetUpdatedComponent(RootComponent);
	pPawnMove->SetRotatedComponent(pMesh);

	// 入力の登録
	// Mapping Context
	pMappingContext = LoadObject<UInputMappingContext>(NULL, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_PlayerAction.IMC_PlayerAction'"));
	// 各種入力
	pMoveInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Move.IA_Move'"));
	pLookInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Look.IA_Look'"));
}

void APlayerPawn::BeginPlay() {
	Super::BeginPlay();

	if (const APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(pMappingContext, 0);
		}
	}

	rLookInput = FRotator::ZeroRotator;
}

void APlayerPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// pCapsule->AddRelativeLocation(temp);

	this->UpdateCameraAngle();

	pPawnMove->UpdatePawnMovement(DeltaTime, pSpringArm->GetComponentRotation());

	UKismetSystemLibrary::PrintString(this, this->GetActorLocation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, rLookInput.ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, pSpringArm->GetComponentRotation().ToString(), true, false, FColor::Red, DeltaTime, TEXT("None"));

	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation()+GetActorUpVector()*100.0f - GetActorForwardVector() * 25.0f, GetActorLocation()+GetActorUpVector() * 100.0f + GetActorForwardVector() * 25.0f, 50.0f, FColor::Blue, DeltaTime * 1.1f, 1.0f);
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation()+GetActorUpVector()*100.0f, GetActorLocation()+GetActorUpVector() * 100.0f + FVector(pSpringArm->GetForwardVector().X, pSpringArm->GetForwardVector().Y, 0.0f) * 25.0f, 50.0f, FColor::Blue, DeltaTime * 1.1f, 1.0f);
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), RootComponent->GetComponentLocation(), RootComponent->GetComponentLocation() + GetActorUpVector() * 100.0f, 50.0f, FColor::White, DeltaTime * 1.1f, 1.0f);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (TObjectPtr<UEnhancedInputComponent> pEnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		pEnhancedInput->BindAction(pMoveInput, ETriggerEvent::Triggered, this, &APlayerPawn::SetMoveInput);
		pEnhancedInput->BindAction(pMoveInput, ETriggerEvent::Completed, this, &APlayerPawn::SetMoveInput);
		pEnhancedInput->BindAction(pLookInput, ETriggerEvent::Triggered, this, &APlayerPawn::SetLookInput);
		pEnhancedInput->BindAction(pLookInput, ETriggerEvent::Completed, this, &APlayerPawn::SetLookInput);

		UKismetSystemLibrary::PrintString(this, TEXT("Success"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("Fatal"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
}

void APlayerPawn::SetMoveInput(const FInputActionValue& val) {
	const FVector2D input = val.Get<FVector2D>();

	pPawnMove->SetInput(input);
}

void APlayerPawn::SetLookInput(const FInputActionValue& val) {
	const FVector2D input = val.Get<FVector2D>();

	rLookInput.Pitch = input.Y;
	rLookInput.Yaw = input.X;
}

void APlayerPawn::UpdateCameraAngle() {
	FRotator cameraAngle = pSpringArm->GetComponentRotation();

	// TODO: ロックオン時の挙動
	if (rLookInput.IsNearlyZero()) return;

	cameraAngle += rLookInput;
	cameraAngle.Pitch = FMath::Clamp(cameraAngle.Pitch, -85.0f, 60.0f);

	pSpringArm->SetWorldRotation(cameraAngle);
}