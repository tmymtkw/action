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

protected:
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<APlayerPawn> playerPawn;

	FVector moveSpeed;

	UPROPERTY(BlueprintReadOnly)
	bool bPowerAttack;

	void GetPlayerPawn();

	UFUNCTION(BlueprintCallable)
	void PlayAttack();


	UFUNCTION(BlueprintCallable)
	void DeactivatePowerAttack();
};
