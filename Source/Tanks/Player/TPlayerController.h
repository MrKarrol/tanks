// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TPlayerController.generated.h"

class ATPlayerPawn;

/**
 * 
 */
UCLASS()
class TANKS_API ATPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	UPROPERTY()
		ATPlayerPawn* PlayerPawn;

public:
	ATPlayerController();
	virtual void SetupInputComponent() override;

protected:
	virtual void BeginPlay() override;
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	
};
