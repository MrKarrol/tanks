// Fill out your copyright notice in the Description page of Project Settings.


#include "TFloor.h"


// Sets default values
ATFloor::ATFloor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ATFloor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATFloor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

