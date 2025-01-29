// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Image.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerPawn.h"
#include "CursorWidget.generated.h"


/**
 * 
 */
UCLASS()
class ACTION_API UCursorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetCursor();


	
protected:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> lockonCursor;

	TObjectPtr<APlayerPawn> pPlayer;

	void GetPlayerPawn();
};
