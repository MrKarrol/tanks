// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerController.h"
#include "TPlayerPawn.h"

ATPlayerController::ATPlayerController()
{
}

void ATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}

void ATPlayerController::MoveForward(float AxisValue)
{
}

void ATPlayerController::MoveRight(float AxisValue)
{
}
