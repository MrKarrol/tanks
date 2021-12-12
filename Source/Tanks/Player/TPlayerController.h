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

protected:
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

private:
	void OnLeftMouseButtonUp();

public:
	FSimpleMulticastDelegate OnMouseButtonUp;
	
};
