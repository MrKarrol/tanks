// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerController.h"
#include "TPlayerPawn.h"

ATPlayerController::ATPlayerController()
{
}

void ATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//InputComponent->BindAxis("MoveForward", this, &ATPlayerController::MoveForward);
	//InputComponent->BindAxis("MoveRight", this, &ATPlayerController::MoveRight);
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	//PlayerPawn = Cast<ATPlayerPawn>(GetPawn());
}

void ATPlayerController::MoveForward(float AxisValue)
{
	//PlayerPawn->MoveForward(AxisValue);
}

void ATPlayerController::MoveRight(float AxisValue)
{
	//PlayerPawn->MoveRight(AxisValue);
}
