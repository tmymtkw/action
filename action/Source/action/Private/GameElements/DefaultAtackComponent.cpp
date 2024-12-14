// Fill out your copyright notice in the Description page of Project Settings.


#include "GameElements/DefaultAtackComponent.h"

// Sets default values
ADefaultAtackComponent::ADefaultAtackComponent()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADefaultAtackComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADefaultAtackComponent::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

