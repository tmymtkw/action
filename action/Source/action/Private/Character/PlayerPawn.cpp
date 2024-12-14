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

// コンストラクタ
APlayerPawn::APlayerPawn() {
	PrimaryActorTick.bCanEverTick = true;

	// 当たり判定
	pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pCapsule->SetCapsuleSize(30.0f, 90.0f);
	pCapsule->SetHiddenInGame(false);
	pCapsule->SetSimulatePhysics(false);
	pCapsule->SetCollisionProfileName("CharacterCollision");
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
	pBlinkInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Blink.IA_Blink'"));
	pSprintInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Sprint.IA_Sprint'"));
	pHealInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Heal.IA_Heal'"));

	// 変数の初期化
	bBlink = false;
	bSprint = false;
	bHeal = false;
	fBlinkTime = 0.0f;
	HP = params.HP - 30.0f;
	AP = 30.0f;
	SP = 50.0f;
}

// 初期化関数
void APlayerPawn::BeginPlay() {
	Super::BeginPlay();

	// Mapping Contextを有効化
	if (const APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(pMappingContext, 0);
		}
	}

	// カメラの移動入力を初期化
	rLookInput = FRotator::ZeroRotator;
}

// フレームごとの処理
void APlayerPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// カメラ位置の更新
	this->UpdateCameraAngle();

	// 速度の更新
	pPawnMove->UpdatePawnMovement(DeltaTime, (0.1f < fBlinkTime), pSpringArm->GetComponentRotation());

	// ブリンク時間の更新
	fBlinkTime = FMathf::Max(fBlinkTime - DeltaTime, 0.0f);

	// SPの回復
	if (fBlinkTime <= 0.0f) {
		SP = FMathf::Min(SP + params.HealSPValue * DeltaTime, params.MaxPower - HP - AP);
	}

	// HPの回復(入力がある時のみ)
	if (bHeal) {
		HealHP(DeltaTime);
	}

	// デバッグ
	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(HP) + TEXT(" ") + FString::SanitizeFloat(AP) + TEXT(" ") + FString::SanitizeFloat(SP), true, false, FColor::White, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, this->GetActorLocation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, pSpringArm->GetComponentRotation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, bSprint ? TEXT("Sprint : true") : TEXT("Sprint : false"), true, false, FColor::Red, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, TEXT("Blink : ") + FString::SanitizeFloat(fBlinkTime), true, false, FColor::Red, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, TEXT("Mouse : ") + rLookInput.ToString(), true, false, FColor::Red, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, TEXT("WASD : ") + pPawnMove->GetInputValue(), true, false, FColor::Red, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::PrintString(this, TEXT("Input"), true, false, FColor::Red, DeltaTime, TEXT("None"));


	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), pMesh->GetComponentLocation() - pMesh->GetRightVector() * 25.0f, pMesh->GetComponentLocation() + pMesh->GetRightVector() * 25.0f, 50.0f, FColor::Blue, DeltaTime * 1.1f, 3.0f);
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), GetActorLocation()+GetActorUpVector()*100.0f, GetActorLocation()+GetActorUpVector() * 100.0f + FVector(pSpringArm->GetForwardVector().X, pSpringArm->GetForwardVector().Y, 0.0f) * 25.0f, 50.0f, FColor::Blue, DeltaTime * 1.1f, .0f);
	//UKismetSystemLibrary::DrawDebugArrow(GetWorld(), RootComponent->GetComponentLocation(), RootComponent->GetComponentLocation() + GetActorUpVector() * 100.0f, 50.0f, FColor::White, DeltaTime * 1.1f, 1.0f);
}

// 入力コンポーネントを設定する関数
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 入力があった時に行う関数のバインド
	if (TObjectPtr<UEnhancedInputComponent> pEnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		pEnhancedInput->BindAction(pMoveInput, ETriggerEvent::Triggered, this, &APlayerPawn::SetMoveInput);
		pEnhancedInput->BindAction(pMoveInput, ETriggerEvent::Completed, this, &APlayerPawn::SetMoveInput);
		pEnhancedInput->BindAction(pLookInput, ETriggerEvent::Triggered, this, &APlayerPawn::SetLookInput);
		pEnhancedInput->BindAction(pLookInput, ETriggerEvent::Completed, this, &APlayerPawn::SetLookInput);
		pEnhancedInput->BindAction(pBlinkInput, ETriggerEvent::Started, this, &APlayerPawn::SetBlinkInput);
		pEnhancedInput->BindAction(pSprintInput, ETriggerEvent::Started, this, &APlayerPawn::SetSprintInput);
		pEnhancedInput->BindAction(pHealInput, ETriggerEvent::Started, this, &APlayerPawn::SetHealInput);
		pEnhancedInput->BindAction(pHealInput, ETriggerEvent::Completed, this, &APlayerPawn::SetHealInput);

		UKismetSystemLibrary::PrintString(this, TEXT("Success"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("Fatal"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
}

// 移動入力を取得する関数
void APlayerPawn::SetMoveInput(const FInputActionValue& val) {
	const FVector2D input = val.Get<FVector2D>();

	// 移動コンポーネントに格納
	pPawnMove->SetInput(input);
}

// カメラ入力を取得する関数
void APlayerPawn::SetLookInput(const FInputActionValue& val) {
	const FVector2D input = val.Get<FVector2D>();

	// 入力の格納
	rLookInput.Pitch = input.Y;
	rLookInput.Yaw = input.X;
}

// ブリンク入力を取得する関数
void APlayerPawn::SetBlinkInput() {
	bBlink = !bBlink;

	// スタミナが不足している場合
	//if (AP + SP < 10.0f) return;
	if (SP < 10.0f) return;

	// すでにブリンク状態の場合
	if (0.0f < fBlinkTime) return;

	// TODO: スタミナ消費
	//AP = FMathf::Max(AP - 15.0f, 0.0f);
	// SPが0にならない場合
	if (params.BlinkPower < SP) {
		SP -= params.BlinkPower;
	}
	// SPが0になる(APを消費する)場合
	else {
		SP = -10.0f;
		//AP = FMathf::Max(AP - (params.BlinkPower - SP), 0.0f);
	}

	// ブリンク時間を設定
	fBlinkTime = params.BLINKTIME;

	// ブリンク方向を設定
	pPawnMove->SetBlinkDirection(pSpringArm->GetComponentRotation());
}

// スプリント入力を取得する関数
void APlayerPawn::SetSprintInput() {
	bSprint = !bSprint;
	pPawnMove->SetSprintInput();
}

// 回復入力を取得する関数
void APlayerPawn::SetHealInput(const FInputActionValue& val) {
	const bool input = val.Get<bool>();

	bHeal = input;
}

// カメラ位置を更新する関数
void APlayerPawn::UpdateCameraAngle() {
	FRotator cameraAngle = pSpringArm->GetComponentRotation();

	// TODO: ロックオン時の挙動
	if (rLookInput.IsNearlyZero()) return;

	// 入力角度を追加
	cameraAngle += rLookInput;
	// 上下方向の位置を制限
	cameraAngle.Pitch = FMathf::Clamp(cameraAngle.Pitch, -85.0f, 60.0f);

	// 更新したカメラ位置に設定
	pSpringArm->SetWorldRotation(cameraAngle);
}

// APを用いて回復を行う関数
void APlayerPawn::HealHP(const float& DeltaTime) {
	// APがない場合
	if (AP <= 0.0f) {
		return;
	}

	// 回復処理
	HP = FMathf::Min(HP + params.HealHPValue * params.HealHPRatio * DeltaTime, params.HPMax);
	AP = FMathf::Max(AP - params.HealHPValue * DeltaTime, 0.0f);
}

// 現在のHPを取得する関数
float APlayerPawn::GetHPVal() { return HP; }

// 現在のAPを取得する関数
float APlayerPawn::GetAPVal() { return AP; }

// 現在のSPを取得する関数
float APlayerPawn::GetSPVal() { return SP; }

// ゲージの最大値を取得する関数
float APlayerPawn::GetMaxPower() { return params.MaxPower; }