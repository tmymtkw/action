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

// �R���X�g���N�^
APlayerPawn::APlayerPawn() {
	PrimaryActorTick.bCanEverTick = true;

	// �����蔻��
	pBody = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pBody->SetCapsuleSize(30.0f, 90.0f);
	pBody->SetHiddenInGame(false);
	pBody->SetSimulatePhysics(false);
	pBody->SetCollisionProfileName("CharacterCollision");
	pBody->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBegin);
	// TODO: Set Root Component
	pBody->SetupAttachment(RootComponent);

	// ���p�R���W����
	pAvoid = CreateDefaultSubobject<USphereComponent>(TEXT("AvoidCollision"));
	pAvoid->SetSphereRadius(125.0f);
	pAvoid->SetHiddenInGame(false);
	pAvoid->SetSimulatePhysics(false);
	pAvoid->SetCollisionProfileName("AvoidCollision");
	pAvoid->OnComponentBeginOverlap.AddDynamic(this, &APlayerPawn::OnOverlapBeginInBlinking);
	pAvoid->SetupAttachment(pBody);

	// ���b�V��
	TObjectPtr<USkeletalMesh> mesh =
		LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	pMesh->SetSkeletalMesh(mesh);
	pMesh->SetupAttachment(pBody);
	pMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// �J�����֘A
	// �J�����̈ړ��͈�
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

	// �J����
	pCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	pCamera->SetupAttachment(pSpringArm);

	// �ړ��R���|�[�l���g
	pPawnMove = CreateDefaultSubobject<UPlayerPawnMovementComponent>(TEXT("PawnMove"));
	pPawnMove->SetUpdatedComponent(RootComponent);
	pPawnMove->SetRotatedComponent(pMesh);

	// ���͂̓o�^
	// Mapping Context
	pMappingContext = LoadObject<UInputMappingContext>(NULL, TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_PlayerAction.IMC_PlayerAction'"));
	// �e�����
	pMoveInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Move.IA_Move'"));
	pLookInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Look.IA_Look'"));
	pBlinkInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Blink.IA_Blink'"));
	pSprintInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Sprint.IA_Sprint'"));
	pHealInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_Heal.IA_Heal'"));
	pCameraLockInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_CameraLock.IA_CameraLock'"));
	pWeakAttackInput = LoadObject<UInputAction>(NULL, TEXT("/Script/EnhancedInput.InputAction'/Game/Input/InputActions/IA_WeakAttack.IA_WeakAttack'"));

	// �ϐ��̏�����
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

// �������֐�
void APlayerPawn::BeginPlay() {
	Super::BeginPlay();

	// Mapping Context��L����
	if (const APlayerController* playerController = Cast<APlayerController>(Controller)) {
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(playerController->GetLocalPlayer())) {
			Subsystem->AddMappingContext(pMappingContext, 0);
		}
	}

	// �J�����̈ړ����͂�������
	rLookInput = FRotator::ZeroRotator;
}

// �t���[�����Ƃ̏���
void APlayerPawn::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	// �J�����ʒu�̍X�V
	this->UpdateCameraAngle();

	// ���x�̍X�V
	pPawnMove->UpdatePawnMovement(DeltaTime, (0.1f < fBlinkTime), bCameraLock, pSpringArm->GetComponentRotation());

	// �u�����N���Ԃ̍X�V
	fBlinkTime = FMathf::Max(fBlinkTime - DeltaTime, 0.0f);

	// SP�̉�
	if (fBlinkTime <= 0.0f) {
		SP = FMathf::Min(SP + params.HealSPValue * DeltaTime, fMaxPower - fHP);
	}

	// HP�̉�(���͂����鎞�̂�)
	if (bHeal) {
		HealHP(DeltaTime);
	}

	// �f�o�b�O
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

// ���̓R���|�[�l���g��ݒ肷��֐�
void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// ���͂����������ɍs���֐��̃o�C���h
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

// �L�����N�^�[���U���ɐڐG�����Ƃ��ɌĂ΂��֐�
void APlayerPawn::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// ��O����
	if (!OtherActor->ActorHasTag(FName("Enemy"))) return;

	if (0.2f < fBlinkTime) return;

	// HP������, AP�ɕϊ�
	fHP = FMathf::Max(fHP - 10.0f, 0.0f);
	AP = FMathf::Min(AP + 10.0f, fMaxPower - fHP);
	UKismetSystemLibrary::PrintString(this, TEXT("Damage"), true, false, FColor::Red, 5.0f, TEXT("None"));
}

// �u�����N���U���ɐڐG�����Ƃ��ɌĂ΂��֐�
void APlayerPawn::OnOverlapBeginInBlinking(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	// ��O����
	if (!OtherActor->ActorHasTag(FName("Enemy"))) return;

	if (fBlinkTime < 0.2f) return;

	// AP���l��
	AP = FMathf::Min(AP + 10.0f, params.MaxPower - fHP);
	UKismetSystemLibrary::PrintString(this, TEXT("Get SP"), true, false, FColor::Red, 5.0f, TEXT("None"));
}


// �ړ����͂��擾����֐�
void APlayerPawn::SetMoveInput(const FInputActionValue& val) {
	const FVector2D input = val.Get<FVector2D>();

	// �ړ��R���|�[�l���g�Ɋi�[
	pPawnMove->SetInput(input);
}

// �J�������͂��擾����֐�
void APlayerPawn::SetLookInput(const FInputActionValue& val) {
	const FVector2D input = val.Get<FVector2D>();

	// ���͂̊i�[
	rLookInput.Pitch = input.Y;
	rLookInput.Yaw = input.X;
}

// �u�����N���͂��擾����֐�
void APlayerPawn::SetBlinkInput() {
	bBlink = !bBlink;

	// �X�^�~�i���s�����Ă���ꍇ
	//if (AP + SP < 10.0f) return;
	if (SP < 10.0f) return;

	// ���łɃu�����N��Ԃ̏ꍇ
	if (0.0f < fBlinkTime) return;

	// TODO: �X�^�~�i����
	//AP = FMathf::Max(AP - 15.0f, 0.0f);
	// SP��0�ɂȂ�Ȃ��ꍇ
	if (params.BlinkPower < SP) {
		SP -= params.BlinkPower;
	}
	// SP��0�ɂȂ�(AP�������)�ꍇ
	else {
		SP = -10.0f;
		//AP = FMathf::Max(AP - (params.BlinkPower - SP), 0.0f);
	}

	// �u�����N���Ԃ�ݒ�
	fBlinkTime = params.BLINKTIME;

	// �u�����N������ݒ�
	pPawnMove->SetBlinkDirection(pSpringArm->GetComponentRotation());
}

// �X�v�����g���͂��擾����֐�
void APlayerPawn::SetSprintInput() {
	bSprint = !bSprint;
	pPawnMove->SetSprintInput();
}

// �񕜓��͂��擾����֐�
void APlayerPawn::SetHealInput(const FInputActionValue& val) {
	const bool input = val.Get<bool>();

	bHeal = input;
}

// ��U�����s���֐�
void APlayerPawn::WeakAttack() {
	//FTransform _SpawnTransform;
	//_SpawnTransform.SetLocation(_Location);
	//_SpawnTransform.SetRotation(_Rotation.Quaternion());

	//auto const _Actor = World->SpawnActorDeferred<ATestActor>(_Class, _SpawnTransform);

	//_Actor->TestParam = 10;		// BeginPlay�O�Ɏ��s����������

	//// �p�����[�^�ݒ�͈ȉ��̂悤�ɖ��O�Ŏw����A�^�����ƂɃ��\�b�h���p�ӂ���Ă���
	//// UKismetSystemLibrary::SetIntPropertyByName(_Actor, FName(TEXT("TestParam")), 10);

	//_Actor->FinishSpawning(_SpawnTransform);	// �X�|�[������

	FVector Location = pMesh->GetComponentLocation() + pMesh->GetRightVector() * 125.0f + pMesh->GetUpVector() * 60.0f;
	FRotator Rotation = pMesh->GetComponentRotation() + FRotator(0.0f, 90.0f, 0.0f);

	TObjectPtr<UWorld> World = GetWorld();
	if (!World) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	TObjectPtr<ADamageCube> Atack = World->SpawnActor<ADamageCube>(Location, Rotation, SpawnParams);
}

// ���U�����s���֐�
void APlayerPawn::StrongAttack() {
	// AP�̏���

	// �U���A�N�^�̃X�|�[��
}

// �J�����ʒu���X�V����֐�
void APlayerPawn::UpdateCameraAngle() {
	FRotator cameraAngle = pSpringArm->GetComponentRotation();

	// ���b�N�I�����ɓG�����ł����ꍇ
	if (bCameraLock && !lockingEnemy) lockingEnemy = nullptr;

	// ���b�N�I����
	if (bCameraLock) {
		cameraAngle = ((lockingEnemy->GetActorLocation() - pBody->GetComponentLocation()) * 0.5f - FVector(0.0f, 0.0f, 100.0f)).Rotation();

		// �㉺�����̈ʒu�𐧌�
		cameraAngle.Pitch = FMathf::Clamp(cameraAngle.Pitch, -30.0f, -15.0f);
	}
	// ���b�N�I�����Ă��Ȃ��ꍇ
	else {
		if (rLookInput.IsNearlyZero()) return;

		// ���͊p�x��ǉ�
		cameraAngle += rLookInput;

		// �㉺�����̈ʒu�𐧌�
		cameraAngle.Pitch = FMathf::Clamp(cameraAngle.Pitch, -85.0f, 60.0f);
	}


	// �X�V�����J�����ʒu�ɐݒ�
	pSpringArm->SetWorldRotation(cameraAngle);
}

// AP��p���ĉ񕜂��s���֐�
void APlayerPawn::HealHP(const float& DeltaTime) {
	// AP���Ȃ��ꍇ
	if (AP <= 0.0f) {
		return;
	}

	// �񕜏���
	fHP = FMathf::Min(fHP + params.HealHPValue * params.HealHPRatio * DeltaTime, params.HPMax);
	AP = FMathf::Max(AP - params.HealHPValue * DeltaTime, 0.0f);
}

// �J�����̃��b�N�I�����s���֐�
void APlayerPawn::UpdateCameraLock() {
	// ���b�N�I������������ꍇ
	if (bCameraLock) {
		bCameraLock = false;
		lockingEnemy = nullptr;

		SetCameraLag(100.0f);
		SetCameraRotationLag(100.0f);
	}
	// ���b�N�I�����s���ꍇ
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

//// ���݂�HP���擾����֐�
//float APlayerPawn::GetHPVal() { return HP; }

// ���݂�AP���擾����֐�
float APlayerPawn::GetAPVal() { return AP; }

// ���݂�SP���擾����֐�
float APlayerPawn::GetSPVal() { return SP; }

//// �Q�[�W�̍ő�l���擾����֐�
//float APlayerPawn::GetMaxPower() { return params.MaxPower; }

// ���b�N���̓G�|�C���^���擾����֐�
TObjectPtr<ABaseCharacterPawn> APlayerPawn::GetLockingEnemy() {
	if (!lockingEnemy) return nullptr;

	return lockingEnemy;
}

bool APlayerPawn::GetIsCameraLock() { return bCameraLock; }