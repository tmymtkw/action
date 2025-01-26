// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

class APlayerPawn;
/**
 * 
 */
UCLASS()
class ACTION_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPlayerAnimInstance();

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	void ActivatePowerAttack();
	//UFUNCTION(BlueprintCallable)
	bool GetPowerAttack();

	void SetMove(const FVector& vInput);

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerPawn> playerPawn;

	UPROPERTY(BlueprintReadOnly)
	FVector moveSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bPowerAttack;

	void GetPlayerPawn();

	UFUNCTION(BlueprintCallable)
	void PlayAttack();


	UFUNCTION(BlueprintCallable)
	void DeactivatePowerAttack();
};
