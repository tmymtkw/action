// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerPawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// コンストラクタ
UPlayerPawnMovementComponent::UPlayerPawnMovementComponent() {
	// Tickを無効化
	// (PawnのTick関数内で速度を更新する関数を呼び出すため)
	PrimaryComponentTick.bCanEverTick = false;
	// 変数の初期化
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

// 移動を行う関数
void UPlayerPawnMovementComponent::UpdatePawnMovement(const float& DeltaTime, bool isBlink, bool isCameraLock, const FRotator& cameraAngle) {
	// 例外処理
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime)) return;

	// 衝突検知用の変数
	FHitResult hitResult;

	// 地面に接しているか判定
	GetIsGround(hitResult);

	// 入力をカメラから見た方向に修正
	GetRotatedInput(cameraAngle);

	// 新しい速度の取得
	Velocity = GetVelocity(DeltaTime, isBlink);

	// 移動方向に応じたメッシュの回転
	// ロックオンしている場合 TODO
	UpdateComponentRotation(DeltaTime, isBlink, isCameraLock);

	// 障害物との衝突を考慮した移動
	if (!Velocity.IsNearlyZero()) {
		SafeMoveUpdatedComponent(Velocity, UpdatedComponent->GetComponentRotation(), true, hitResult);

		if (hitResult.IsValidBlockingHit()) SlideAlongSurface(Velocity, 1.f - hitResult.Time, hitResult.Normal, hitResult);
	}

	// 移動速度の更新
	UpdateComponentVelocity();

	// デバッグ
	//UKismetSystemLibrary::PrintString(this, Velocity.ToString(), true, false, FColor::Red, DeltaTime, TEXT("None"));
	UKismetSystemLibrary::DrawDebugArrow(GetWorld(), UpdatedComponent->GetComponentLocation(), UpdatedComponent->GetComponentLocation() + vBlinkDir * 100.0f, 50.0f, FColor::Red, DeltaTime * 1.1f, 1.0f);

}

// コンポーネントを移動方向に回転させる関数
void UPlayerPawnMovementComponent::UpdateComponentRotation(const float& DeltaTime, bool isBlink, bool isCameraLock) {
	// TODO: 回転させない例外処理

	// 角度の差
	FRotator rotationDifference = vRotatedInput.Rotation() - rotatedComponent->GetRightVector().Rotation();

	// [-180.0f, 180.0f]に収まるように修正
	if (rotationDifference.Yaw < -180.0f) rotationDifference.Yaw += 360.0f;
	else if (180.0f < rotationDifference.Yaw) rotationDifference.Yaw -= 360.0f;
	// 1フレームで回転させる量
	float deltaDegree = rotationDifference.Yaw * 10.0f * DeltaTime;

	// コンポーネントを回転
	// 移動方向をすでに向いている場合
	if (rotationDifference.Yaw == 0.0f) {
		return;
	}
	// 角度の差がほぼ0の場合
	else if (FMathf::Abs(rotationDifference.Yaw) < 1.0f) {
		rotatedComponent->SetWorldRotation(vRotatedInput.Rotation() + FRotator(0.0f, -90.0f, 0.0f));
	}
	// 通常
	else {
		rotatedComponent->AddRelativeRotation(FRotator(0.0f, deltaDegree, 0.0f));
	}
}

// 回転させるコンポーネントを設定する関数
void UPlayerPawnMovementComponent::SetRotatedComponent(TObjectPtr<USceneComponent> newRotatedComponent) {
	rotatedComponent = newRotatedComponent;
}

// 移動入力を取得する関数
void UPlayerPawnMovementComponent::SetInput(const FVector2D& val) {
	vInput.X = val.X;
	vInput.Y = val.Y;

	// 正規化
	// (X,Yが同時に入力されていると絶対値が異なる)
	vInput.Normalize(1.0);
}

// スプリント入力を取得する関数
void UPlayerPawnMovementComponent::SetSprintInput() {
	bSprint = !bSprint;
}

// ブリンク用のベクトルを設定する関数
void UPlayerPawnMovementComponent::SetBlinkDirection(const FRotator& cameraAngle) {
	// 移動方向が入力されていない場合
	if (vInput.IsZero()) {
		vBlinkDir = -1.0f * rotatedComponent->GetRightVector();
	}
	// 移動方向が入力されている場合
	else {
		vBlinkDir = vRotatedInput.GetSafeNormal();
	}
}

// 接地判定を行う関数
void UPlayerPawnMovementComponent::GetIsGround(FHitResult& hitResult) {
	// 地面方向にトレース
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

	// 地面の法線ベクトルを取得
	if (!bIsGround) return;
	hitGround = hitResult.Normal;
}

// 入力の向きを修正する関数
void UPlayerPawnMovementComponent::GetRotatedInput(const FRotator& cameraAngle) {
	// 例外処理
	if (vInput.IsZero()) return;

	// カメラの向きを正面として修正
	vRotatedInput = vInput.RotateAngleAxis(cameraAngle.Yaw, FVector::UpVector);
}

// 速度を計算する関数
FVector UPlayerPawnMovementComponent::GetVelocity(const float& DeltaTime, bool& isBlink) {
	// 新しい速度
	FVector newVelocity = FVector::Zero();

	// XY方向の速度を更新
	if (!vInput.IsZero()) {
		newVelocity += vRotatedInput * (params.WALK + bSprint * params.SPRINT) * DeltaTime;
	}
	//  地面の角度を反映
	newVelocity -= hitGround.GetSafeNormal() * (newVelocity | hitGround);

	// Z方向の速度を更新
	// 空中にいる場合
	if (!bIsGround) {
		fJumpSpeed = FMathf::Max(fJumpSpeed - params.FALL * DeltaTime * !isBlink, params.FALLMAX);
	}
	// 地上にいる場合
	else if (fJumpSpeed < 0.0f) {
		fJumpSpeed = 0.0f;
	}
	// 落下している時にXY方向の移動を制限
	if (fJumpSpeed < 0.0f) {
		// TODO
	}
	// 落下速度の更新
	newVelocity.Z += fJumpSpeed;

	// ブリンク中の場合
	if (isBlink) {
		newVelocity = vBlinkDir * params.BLINK * DeltaTime;
	}

	return newVelocity;
}

FString UPlayerPawnMovementComponent::GetInputValue() {
	return vInput.ToString();
}