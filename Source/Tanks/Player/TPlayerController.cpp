// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerController.h"


ATPlayerController::ATPlayerController()
{
	bEnableClickEvents = true;
}

void ATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// if (InputComponent)
	// {
	// 	InputComponent->BindKey(EKeys::LeftMouseButton, IE_Released,
	// 		this, &ATPlayerController::OnLeftMouseButtonUp);
	// }
}

void ATPlayerController::OnLeftMouseButtonUp()
{
	if (OnMouseButtonUp.IsBound())
		OnMouseButtonUp.Broadcast();
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}
