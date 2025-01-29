// Fill out your copyright notice in the Description page of Project Settings.


#include "Gamemode/GameModeTitleMenu.h"
#include "UI/TitleMenuHUD.h"

AGameModeTitleMenu::AGameModeTitleMenu() {
	HUDClass = ATitleMenuHUD::StaticClass();
}