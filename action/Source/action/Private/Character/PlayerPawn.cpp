// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/PlayerPawnMovementComponent.h"
#include "GameElements/DamageCube.h"

// コンストラクタ
APlayerPawn::APlayerPawn() {
	PrimaryActorTick.bCanEverTick = true;

	// 当たり判定
	pBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pBody->SetCapsuleSize(30.0f, 90.0f);
	pBody->SetHiddenInGame(false);
	pBody->SetSimulatePhysics(false);
	pBody->SetCollisionProfileName("CharacterCollision");
	pBody->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
	// TODO: Set Root Component
	pBody->SetupAttachment(RootComponent);

	// 回避用コリジョン
	pAvoid = CreateDefaultSubobject<USphereComponent>(TEXT("AvoidCollision"));
	pAvoid->SetSphereRadius(125.0f);
	pAvoid->SetHiddenInGame(false);
	pAvoid->SetSimulatePhysics(false);
	pAvoid->SetCollisionProfileName("AvoidCollision");
	pAvoid->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBeginInBlinking);
	pAvoid->SetupAttachment(pBody);

	// メッシュ
	TObjectPtr<USkeletalMesh> mesh =
		LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	pMesh->SetSkeletalMesh(mesh);
	pMesh->SetupAttachment(pBody);
	pMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// カメラ関連
	// カメラの移動範囲
	pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	pSpringArm->SetupAttachment(pBody);
	pSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 80.0f), FRotator(-10.0f, 0.0f, 0.0f));
	pSpringArm->TargetArmLength = 300.f;
	pSpringArm->bInheritPitch = false;
	pSpringArm->bInheritYaw = false;
	pSpringArm->bInheritRoll = false;
	pSpringArm->bEnableCameraLag = true;
	pSpringArm->bEnableCameraRotationLag = true;
	SetCameraLag(100.0f);
	SetCameraRotationLag(100.0f);

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
	pCameraLockInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_CameraLock.IA_CameraLock'"));
	pWeakAttackInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_WeakAttack.IA_WeakAttack'"));

	// 変数の初期化
	bBlink = false;
	bSprint = false;
	bHeal = false;
	bCameraLock = false;
	fBlinkTime = 0.0f;
	fHP = params.HP - 30.0f;
	fMaxPower = params.MaxPower;
	AP = 30.0f;
	SP = 50.0f;

	traceObjects = { UEngineTypes::ConvertToObjectType(ECC_Pawn) };
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
	pPawnMove->UpdatePawnMovement(DeltaTime, (0.1f < fBlinkTime), bCameraLock, pSpringArm->GetComponentRotation());

	// ブリンク時間の更新
	fBlinkTime = FMathf::Max(fBlinkTime - DeltaTime, 0.0f);

	// SPの回復
	if (fBlinkTime <= 0.0f) {
		SP = FMathf::Min(SP + params.HealSPValue * DeltaTime, fMaxPower - fHP);
	}

	// HPの回復(入力がある時のみ)
	if (bHeal) {
		HealHP(DeltaTime);
	}

	// デバッグ
	//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(fHP) + TEXT(" ") + FString::SanitizeFloat(AP) + TEXT(" ") + FString::SanitizeFloat(SP), true, false, FColor::White, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, this->GetActorLocation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, pSpringArm->GetComponentRotation().ToString(), true, false, FColor::White, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, bSprint ? TEXT("Sprint : true") : TEXT("Sprint : false"), true, false, FColor::Red, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, TEXT("Blink : ") + FString::SanitizeFloat(fBlinkTime), true, false, FColor::Red, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, TEXT("Mouse : ") + rLookInput.ToString(), true, false, FColor::Red, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, TEXT("WASD : ") + pPawnMove->GetInputValue(), true, false, FColor::Red, DeltaTime, TEXT("None"));
	//UKismetSystemLibrary::PrintString(this, TEXT("Input"), true, false, FColor::Red, DeltaTime, TEXT("None"));


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
		pEnhancedInput->BindAction(pCameraLockInput, ETriggerEvent::Started, this, &APlayerPawn::UpdateCameraLock);
		pEnhancedInput->BindAction(pWeakAttackInput, ETriggerEvent::Started, this, &APlayerPawn::WeakAttack);

		UKismetSystemLibrary::PrintString(this, TEXT("Success"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("Fatal"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
}

// キャラクターが攻撃に接触したときに呼ばれる関数
void APlayerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// 例外処理
	if (!OtherActor->ActorHasTag(FName("Enemy"))) return;

	if (0.2f < fBlinkTime) return;

	// HPが減少, APに変換
	fHP = FMathf::Max(fHP - 10.0f, 0.0f);
	AP = FMathf::Min(AP + 10.0f, fMaxPower - fHP);
	UKismetSystemLibrary::PrintString(this, TEXT("Damage"), true, false, FColor::Red, 5.0f, TEXT("None"));
}

// ブリンク中攻撃に接触したときに呼ばれる関数
void APlayerPawn::OnOverlapBeginInBlinking(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// 例外処理
	if (!OtherActor->ActorHasTag(FName("Enemy"))) return;

	if (fBlinkTime < 0.2f) return;

	// APを獲得
	AP = FMathf::Min(AP + 10.0f, params.MaxPower - fHP);
	UKismetSystemLibrary::PrintString(this, TEXT("Get SP"), true, false, FColor::Red, 5.0f, TEXT("None"));
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

// 弱攻撃を行う関数
void APlayerPawn::WeakAttack() {
	//FTransform _SpawnTransform;
	//_SpawnTransform.SetLocation(_Location);
	//_SpawnTransform.SetRotation(_Rotation.Quaternion());

	//auto const _Actor = World->SpawnActorDeferred<ATestActor>(_Class, _SpawnTransform);

	//_Actor->TestParam = 10;		// BeginPlay前に実行したい処理

	//// パラメータ設定は以下のように名前で指定も可、型名ごとにメソッドが用意されている
	//// UKismetSystemLibrary::SetIntPropertyByName(_Actor, FName(TEXT("TestParam")), 10);

	//_Actor->FinishSpawning(_SpawnTransform);	// スポーン処理

	FVector Location = pMesh->GetComponentLocation() + pMesh->GetRightVector() * 125.0f + pMesh->GetUpVector() * 60.0f;
	FRotator Rotation = pMesh->GetComponentRotation() + FRotator(0.0f, 90.0f, 0.0f);

	TObjectPtr<UWorld> World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	TObjectPtr<ADamageCube> Atack = World->SpawnActor<ADamageCube>(Location, Rotation, SpawnParams);
}

// 強攻撃を行う関数
void APlayerPawn::StrongAttack() {
	// APの消費

	// 攻撃アクタのスポーン
}

// カメラ位置を更新する関数
void APlayerPawn::UpdateCameraAngle() {
	FRotator cameraAngle = pSpringArm->GetComponentRotation();

	// ロックオン中に敵が消滅した場合
	if (bCameraLock && !lockingEnemy) lockingEnemy = nullptr;

	// ロックオン時
	if (bCameraLock) {
		cameraAngle = ((lockingEnemy->GetActorLocation() - pBody->GetComponentLocation()) * 0.5f - FVector(0.0f, 0.0f, 100.0f)).Rotation();

		// 上下方向の位置を制限
		cameraAngle.Pitch = FMathf::Clamp(cameraAngle.Pitch, -30.0f, -15.0f);
	}
	// ロックオンしていない場合
	else {
		if (rLookInput.IsNearlyZero()) return;

		// 入力角度を追加
		cameraAngle += rLookInput;

		// 上下方向の位置を制限
		cameraAngle.Pitch = FMathf::Clamp(cameraAngle.Pitch, -85.0f, 60.0f);
	}


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
	fHP = FMathf::Min(fHP + params.HealHPValue * params.HealHPRatio * DeltaTime, params.HPMax);
	AP = FMathf::Max(AP - params.HealHPValue * DeltaTime, 0.0f);
}

// カメラのロックオンを行う関数
void APlayerPawn::UpdateCameraLock() {
	// ロックオンを解除する場合
	if (bCameraLock) {
		bCameraLock = false;
		lockingEnemy = nullptr;

		SetCameraLag(100.0f);
		SetCameraRotationLag(100.0f);
	}
	// ロックオンを行う場合
	else {
		TArray<FHitResult> HitResults;
		FVector traceStart = pBody->GetComponentLocation() + FVector::UpVector * 80.0f;
		FVector traceEnd = traceStart + pCamera->GetForwardVector() * 1000.0f;
		FRotator rot = pCamera->GetComponentRotation();

		bCameraLock = UKismetSystemLibrary::BoxTraceMultiForObjects(
			GetWorld(),
			traceStart,
			traceEnd,
			FVector(0.0f, 500.0f, 300.0f),
			rot,
			traceObjects,
			false,
			TArray<AActor*>{ this },
			EDrawDebugTrace::ForDuration,
			HitResults,
			true,
			FColor::Green,
			FColor::Red
		);

		if (!bCameraLock) return;

		lockingEnemy = Cast<ABaseCharacterPawn>(HitResults.Top().GetActor());
		SetCameraLag(5.0f);
		SetCameraRotationLag(5.0f);
		
		if (!GEngine) return;
		for (auto& hit : HitResults) {
			AActor* enemy = hit.GetActor();
			UKismetSystemLibrary::DrawDebugString(GetWorld(), enemy->GetActorLocation(), FString::Printf(TEXT("Enemy")), nullptr, FLinearColor::Red, 3.0f);
		}
	}
}

void APlayerPawn::SetCameraLag(float val) {
	pSpringArm->CameraLagSpeed = val;
}

void APlayerPawn::SetCameraRotationLag(float val) {
	pSpringArm->CameraRotationLagSpeed = val;
}

//// 現在のHPを取得する関数
//float APlayerPawn::GetHPVal() { return HP; }

// 現在のAPを取得する関数
float APlayerPawn::GetAPVal() { return AP; }

// 現在のSPを取得する関数
float APlayerPawn::GetSPVal() { return SP; }

//// ゲージの最大値を取得する関数
//float APlayerPawn::GetMaxPower() { return params.MaxPower; }

// ロック中の敵ポインタを取得する関数
TObjectPtr<ABaseCharacterPawn> APlayerPawn::GetLockingEnemy() {
	if (!lockingEnemy) return nullptr;

	return lockingEnemy;
}

bool APlayerPawn::GetIsCameraLock() { return bCameraLock; }