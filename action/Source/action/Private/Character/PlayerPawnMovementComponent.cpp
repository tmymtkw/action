// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerPawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

UPlayerPawnMovementComponent::UPlayerPawnMovementComponent() {
	PrimaryComponentTick.bCanEverTick = false;
	vInput = FVector::Zero();
	vRotatedInput = FVector::Zero();
	Velocity = FVector::Zero();
}

void UPlayerPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	UKismetSystemLibrary::PrintString(this, TEXT("PawnMovement Tick"), true, false, FColor::Red, DeltaTime, TEXT("None"));
}

void UPlayerPawnMovementComponent::UpdatePawnMovement(float DeltaTime, const FRotator& cameraAngle) {
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) return;

	FHitResult hitResult;

	GetRotatedInput(cameraAngle);

	Velocity = GetVelocity(DeltaTime);

	UpdateComponentRotation(DeltaTime);

	if (!Velocity.IsNearlyZero()) {
		SafeMoveUpdatedComponent(Velocity, UpdatedComponent->GetComponentRotation(), true, hitResult);

		if (hitResult.IsValidBlockingHit()) SlideAlongSurface(Velocity, 1.f - hitResult.Time, hitResult.Normal, hitResult);
	}

	UpdateComponentVelocity();

	UKismetSystemLibrary::PrintString(this, Velocity.ToString(), true, false, FColor::Red, DeltaTime, TEXT("None"));

}

void UPlayerPawnMovementComponent::UpdateComponentRotation(float DeltaTime) {
	// TODO: 回転させない例外処理

	FRotator rotationDifference = vRotatedInput.Rotation() - rotatedComponent->GetRightVector().Rotation();

	if (rotationDifference.Yaw < -180.0f) rotationDifference.Yaw += 360.0f;
	else if (180.0f < rotationDifference.Yaw) rotationDifference.Yaw -= 360.0f;
	float deltaDegree = FMathf::Abs(rotationDifference.Yaw) * 4.0f * DeltaTime;

	// コンポーネントを回転
	if (rotationDifference.Yaw == 0.0f) {
		return;
	}
	else if (FMathf::Abs(rotationDifference.Yaw) < 1.0f) {
		rotatedComponent->SetWorldRotation(vRotatedInput.Rotation() + FRotator(0.0f, -90.0f, 0.0f));
	}
	else {
		if (rotationDifference.Yaw < 0.0f) deltaDegree *= -1;
		rotatedComponent->AddRelativeRotation(FRotator(0.0f, deltaDegree, 0.0f));
	}
}

void UPlayerPawnMovementComponent::SetRotatedComponent(TObjectPtr<USceneComponent> newRotatedComponent) {
	rotatedComponent = newRotatedComponent;
}

void UPlayerPawnMovementComponent::SetInput(const FVector2D& val) {
	vInput.X = val.X;
	vInput.Y = val.Y;

	vInput.Normalize(1.0);
}

void UPlayerPawnMovementComponent::GetRotatedInput(const FRotator& cameraAngle) {
	if (vInput.IsZero()) return;

	vRotatedInput = vInput.RotateAngleAxis(cameraAngle.Yaw, FVector::UpVector);
}

FVector UPlayerPawnMovementComponent::GetVelocity(float DeltaTime) {
	FVector newVelocity = FVector::Zero();

	// TODO: パラメータ設定
	if (!vInput.IsZero()) {
		newVelocity += vRotatedInput * 1000.0f * DeltaTime;
	}

	return newVelocity;
}