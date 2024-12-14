// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "InGameHUD.generated.h"

class UUserWidget;
/**
 * 
 */
UCLASS()
class ACTION_API AInGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:

protected:
	virtual void BeginPlay() override;


};
