// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ATPlayerController();

private:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	void OnLeftMouseButtonUp();

	void ShowMinimap();
	void ShowTurretHelpers();
	void ShowInventories();
	void ShowPauseMenu();
	void ShowBrowseWidget();

	void TankStartFire();
	void TankStopFire();
	void TankAlternateFire();
	void TankSwapGuns();

	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void MoveTurretUp(float Axis);
	void MoveTurretRight(float Axis);

	void Interact();
	void ToggleJournal();

public:
	FSimpleMulticastDelegate OnMouseButtonUp;
	
};
