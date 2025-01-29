// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/DefaultAtackActor.h"

// Sets default values
ADefaultAtackActor::ADefaultAtackActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	damage = 0.0f;
}

// Called when the game starts or when spawned
void ADefaultAtackActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultAtackActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float ADefaultAtackActor::GetDamageValue() { return damage; }

void ADefaultAtackActor::AddActorTag(FName newTag) {
	Tags.Add(newTag);
}

void ADefaultAtackActor::SetDamageValue(float val) {
	damage = val;
}