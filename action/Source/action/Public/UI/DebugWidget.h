// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "DebugWidget.generated.h"

class APlayerPawn;
/**
 * 
 */
UCLASS()
class ACTION_API UDebugWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

	//virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	TObjectPtr<APlayerPawn> pPlayer;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> locationInformation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> rotationInformation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> hpInformation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> apInformation;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> spInformation;

	UFUNCTION()
	FText GetLocationInformation();

	UFUNCTION()
	FText GetRotationInformation();

	UFUNCTION()
	FText GetHPInformation();

	UFUNCTION()
	FText GetAPInformation();

	UFUNCTION()
	FText GetSPInformation();

	void GetPlayerPawn();

	FName zero = FName("0.0f");
};
