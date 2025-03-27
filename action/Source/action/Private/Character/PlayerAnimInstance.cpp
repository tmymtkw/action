// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerAnimInstance.h"
#include "Character/PlayerPawn.h"

UPlayerAnimInstance::UPlayerAnimInstance() {
	bPowerAttack = false;
	bDeath = false;
	moveSpeed = FVector::Zero();
}

void UPlayerAnimInstance::NativeInitializeAnimation() {
	if (!playerPawn) GetPlayerPawn();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds) {
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (playerPawn) return;

	GetPlayerPawn();
}

void UPlayerAnimInstance::ActivateDeath() {
	bDeath = true;
}

bool UPlayerAnimInstance::GetPowerAttack() {
	return bPowerAttack;
}

void UPlayerAnimInstance::SetMove(const FVector& vInput) {
	moveSpeed = vInput;
}

void UPlayerAnimInstance::PlayAttack() {
	if (!playerPawn) return;

	playerPawn->PowerAttack();
}

void UPlayerAnimInstance::GetPlayerPawn() {
	playerPawn = Cast<APlayerPawn>(TryGetPawnOwner());
}

void UPlayerAnimInstance::ActivatePowerAttack() {
	bPowerAttack = true;
}

void UPlayerAnimInstance::DeactivatePowerAttack() {
	bPowerAttack = false;

}

void UPlayerAnimInstance::ActivateBlink(bool isLock) {
	bBlink = true;

	if (isLock) {
		blinkDir = moveSpeed;
	}
	else {
		blinkDir = FVector::ForwardVector;
	}
}

void UPlayerAnimInstance::DeactivateBlink() {
	bBlink = false;
}