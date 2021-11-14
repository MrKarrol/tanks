// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IScoreTaker.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIScoreTaker : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TANKS_API IIScoreTaker
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void TakeScore(float Score) = 0;
};
