// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/PlayerCameraShake.h"

UPlayerCameraShake::UPlayerCameraShake() {
	this->OscillationDuration = 0.25f;
	this->OscillationBlendInTime = 0.05f;
	this->OscillationBlendOutTime = 0.05f;

	float yawAmp = FMath::FRandRange(0.2f, 1.0f);
	this->RotOscillation.Yaw.Amplitude = yawAmp;
	this->RotOscillation.Pitch.Amplitude = 1.0f - yawAmp;
	this->RotOscillation.Yaw.Frequency = 30.f;
	this->RotOscillation.Pitch.Frequency = 30.f;
}