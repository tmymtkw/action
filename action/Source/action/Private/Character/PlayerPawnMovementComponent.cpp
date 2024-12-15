// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerPawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// �R���X�g���N�^
UPlayerPawnMovementComponent::UPlayerPawnMovementComponent() {
	// Tick�𖳌���
	// (Pawn��Tick�֐����ő��x���X�V����֐����Ăяo������)
	PrimaryComponentTick.bCanEverTick = false;
	// �ϐ��̏�����
	vInput = FVector::Zero();
	vRotatedInput = FVector::Zero();
	Velocity = FVector::Zero();
	bIsGround = false;
	bSprint = false;
	fJumpSpeed = 0.0f;
}

void UPlayerPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	UKismetSystemLibrary::PrintString(this, TEXT("Error: PawnMovement Tick Enabled"), true, false, FColor::Red, DeltaTime, TEXT("None"));
}

// �ړ����s���֐�
void UPlayerPawnMovementComponent::UpdatePawnMovement(const float& DeltaTime, bool isBlink, bool isCameraLock, const FRotator& cameraAngle) {
	// ��O����
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) return;

	// �Փˌ��m�p�̕ϐ�
	FHitResult hitResult;

	// �n�ʂɐڂ��Ă��邩����
	GetIsGround(hitResult);

	// ���͂��J�������猩�������ɏC��
	GetRotatedInput(cameraAngle);

	// �V�������x�̎擾
	Velocity = GetVelocity(DeltaTime, isBlink);

	// �ړ������ɉ��������b�V���̉�]
	// ���b�N�I�����Ă���ꍇ TODO
	UpdateComponentRotation(DeltaTime, isBlink, isCameraLock);

	// ��Q���Ƃ̏Փ˂��l�������ړ�
	if (!Velocity.IsNearlyZero()) {
		SafeMoveUpdatedComponent(Velocity, UpdatedComponent->GetComponentRotation(), true, hitResult);

		if (hitResult.IsValidBlockingHit()) SlideAlongSurface(Velocity, 1.f - hitResult.Time, hitResult.Normal, hitResult);
	}

	// �ړ����x�̍X�V
	UpdateComponentVelocity();

	// �f�o�b�O
	//UKismetSystemLibrary::PrintString(this, Velocity.ToString(), true, false, FColor::Red, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + vBlinkDir * 100.0f, 50.0f, FColor::Red, DeltaTime * 1.1f, 1.0f);

}

// �R���|�[�l���g���ړ������ɉ�]������֐�
void UPlayerPawnMovementComponent::UpdateComponentRotation(const float& DeltaTime, bool isBlink, bool isCameraLock) {
	// TODO: ��]�����Ȃ���O����

	// �p�x�̍�
	FRotator rotationDifference = vRotatedInput.Rotation() - rotatedComponent->GetRightVector().Rotation();

	// [-180.0f, 180.0f]�Ɏ��܂�悤�ɏC��
	if (rotationDifference.Yaw < -180.0f) rotationDifference.Yaw += 360.0f;
	else if (180.0f < rotationDifference.Yaw) rotationDifference.Yaw -= 360.0f;
	// 1�t���[���ŉ�]�������
	float deltaDegree = rotationDifference.Yaw * 10.0f * DeltaTime;

	// �R���|�[�l���g����]
	// �ړ����������łɌ����Ă���ꍇ
	if (rotationDifference.Yaw == 0.0f) {
		return;
	}
	// �p�x�̍����ق�0�̏ꍇ
	else if (FMathf::Abs(rotationDifference.Yaw) < 1.0f) {
		rotatedComponent->SetWorldRotation(vRotatedInput.Rotation() + FRotator(0.0f, -90.0f, 0.0f));
	}
	// �ʏ�
	else {
		rotatedComponent->AddRelativeRotation(FRotator(0.0f, deltaDegree, 0.0f));
	}
}

// ��]������R���|�[�l���g��ݒ肷��֐�
void UPlayerPawnMovementComponent::SetRotatedComponent(TObjectPtr<USceneComponent> newRotatedComponent) {
	rotatedComponent = newRotatedComponent;
}

// �ړ����͂��擾����֐�
void UPlayerPawnMovementComponent::SetInput(const FVector2D& val) {
	vInput.X = val.X;
	vInput.Y = val.Y;

	// ���K��
	// (X,Y�������ɓ��͂���Ă���Ɛ�Βl���قȂ�)
	vInput.Normalize(1.0);
}

// �X�v�����g���͂��擾����֐�
void UPlayerPawnMovementComponent::SetSprintInput() {
	bSprint = !bSprint;
}

// �u�����N�p�̃x�N�g����ݒ肷��֐�
void UPlayerPawnMovementComponent::SetBlinkDirection(const FRotator& cameraAngle) {
	// �ړ����������͂���Ă��Ȃ��ꍇ
	if (vInput.IsZero()) {
		vBlinkDir = -1.0f * rotatedComponent->GetRightVector();
	}
	// �ړ����������͂���Ă���ꍇ
	else {
		vBlinkDir = vRotatedInput.GetSafeNormal();
	}
}

// �ڒn������s���֐�
void UPlayerPawnMovementComponent::GetIsGround(FHitResult& hitResult) {
	// �n�ʕ����Ƀg���[�X
	bIsGround = UKismetSystemLibrary::SphereTraceSingle(
		GetWorld(),
		UpdatedComponent->GetComponentLocation() - FVector::UpVector * 65.0f,
		UpdatedComponent->GetComponentLocation() - FVector::UpVector * 75.0f,
		20.0f,
		UEngineTypes::ConvertToTraceType(ECC_WorldStatic),
		false,
		TArray<AActor*>{ PawnOwner },
		EDrawDebugTrace::ForOneFrame,
		hitResult,
		true,
		FColor::Green,
		FColor::Red
	);

	// �n�ʂ̖@���x�N�g�����擾
	if (!bIsGround) return;
	hitGround = hitResult.Normal;
}

// ���͂̌������C������֐�
void UPlayerPawnMovementComponent::GetRotatedInput(const FRotator& cameraAngle) {
	// ��O����
	if (vInput.IsZero()) return;

	// �J�����̌����𐳖ʂƂ��ďC��
	vRotatedInput = vInput.RotateAngleAxis(cameraAngle.Yaw, FVector::UpVector);
}

// ���x���v�Z����֐�
FVector UPlayerPawnMovementComponent::GetVelocity(const float& DeltaTime, bool& isBlink) {
	// �V�������x
	FVector newVelocity = FVector::Zero();

	// XY�����̑��x���X�V
	if (!vInput.IsZero()) {
		newVelocity += vRotatedInput * (params.WALK + bSprint * params.SPRINT) * DeltaTime;
	}
	//  �n�ʂ̊p�x�𔽉f
	newVelocity -= hitGround.GetSafeNormal() * (newVelocity | hitGround);

	// Z�����̑��x���X�V
	// �󒆂ɂ���ꍇ
	if (!bIsGround) {
		fJumpSpeed = FMathf::Max(fJumpSpeed - params.FALL * DeltaTime * !isBlink, params.FALLMAX);
	}
	// �n��ɂ���ꍇ
	else if (fJumpSpeed < 0.0f) {
		fJumpSpeed = 0.0f;
	}
	// �������Ă��鎞��XY�����̈ړ��𐧌�
	if (fJumpSpeed < 0.0f) {
		// TODO
	}
	// �������x�̍X�V
	newVelocity.Z += fJumpSpeed;

	// �u�����N���̏ꍇ
	if (isBlink) {
		newVelocity = vBlinkDir * params.BLINK * DeltaTime;
	}

	return newVelocity;
}

FString UPlayerPawnMovementComponent::GetInputValue() {
	return vInput.ToString();
}