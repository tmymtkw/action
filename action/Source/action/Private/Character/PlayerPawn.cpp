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

// �R���X�g���N�^
APlayerPawn::APlayerPawn() {
	PrimaryActorTick.bCanEverTick = true;

	// �����蔻��
	pCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	pCapsule->SetCapsuleSize(30.0f, 90.0f);
	pCapsule->SetHiddenInGame(false);
	pCapsule->SetSimulatePhysics(false);
	pCapsule->SetCollisionProfileName("CharacterCollision");
	// TODO: Set Root Component
	pCapsule->SetupAttachment(RootComponent);

	// ���b�V��
	TObjectPtr<USkeletalMesh> mesh =
		LoadObject<USkeletalMesh>(nullptr, TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'"));
	pMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	pMesh->SetSkeletalMesh(mesh);
	pMesh->SetupAttachment(pCapsule);
	pMesh->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// �J�����֘A
	// �J�����̈ړ��͈�
	pSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	pSpringArm->SetupAttachment(pCapsule);
	pSpringArm->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 80.0f), FRotator(-10.0f, 0.0f, 0.0f));
	pSpringArm->TargetArmLength = 300.f;
	pSpringArm->bInheritPitch = false;
	pSpringArm->bInheritYaw = false;
	pSpringArm->bInheritRoll = false;

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

	// �ϐ��̏�����
	bBlink = false;
	bSprint = false;
	bHeal = false;
	fBlinkTime = 0.0f;
	HP = params.HP - 30.0f;
	AP = 30.0f;
	SP = 50.0f;
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
	pPawnMove->UpdatePawnMovement(DeltaTime, (0.1f < fBlinkTime), pSpringArm->GetComponentRotation());

	// �u�����N���Ԃ̍X�V
	fBlinkTime = FMathf::Max(fBlinkTime - DeltaTime, 0.0f);

	// SP�̉�
	if (fBlinkTime <= 0.0f) {
		SP = FMathf::Min(SP + params.HealSPValue * DeltaTime, params.MaxPower - HP - AP);
	}

	// HP�̉�(���͂����鎞�̂�)
	if (bHeal) {
		HealHP(DeltaTime);
	}

	// �f�o�b�O
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

		UKismetSystemLibrary::PrintString(this, TEXT("Success"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("Fatal"), true, false, FColor::Red, 5.0f, TEXT("None"));
	}
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

// �J�����ʒu���X�V����֐�
void APlayerPawn::UpdateCameraAngle() {
	FRotator cameraAngle = pSpringArm->GetComponentRotation();

	// TODO: ���b�N�I�����̋���
	if (rLookInput.IsNearlyZero()) return;

	// ���͊p�x��ǉ�
	cameraAngle += rLookInput;
	// �㉺�����̈ʒu�𐧌�
	cameraAngle.Pitch = FMathf::Clamp(cameraAngle.Pitch, -85.0f, 60.0f);

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
	HP = FMathf::Min(HP + params.HealHPValue * params.HealHPRatio * DeltaTime, params.HPMax);
	AP = FMathf::Max(AP - params.HealHPValue * DeltaTime, 0.0f);
}

// ���݂�HP���擾����֐�
float APlayerPawn::GetHPVal() { return HP; }

// ���݂�AP���擾����֐�
float APlayerPawn::GetAPVal() { return AP; }

// ���݂�SP���擾����֐�
float APlayerPawn::GetSPVal() { return SP; }

// �Q�[�W�̍ő�l���擾����֐�
float APlayerPawn::GetMaxPower() { return params.MaxPower; }