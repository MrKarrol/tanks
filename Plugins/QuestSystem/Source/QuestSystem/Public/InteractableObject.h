// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractableObject.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractableObject : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class QUESTSYSTEM_API IInteractableObject
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnInteractionFinished, AActor * /*InteractableObject*/,
		AActor * /*Actor, interacted with object*/)

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(AActor * ActorInteractedWithObject);
	UFUNCTION()
	virtual void Interact_Implementation(AActor *ActorInteractedWithObject) {}

	virtual void NotifyInteractionFinished(AActor *InteractableObject, AActor * ActorInteractedWithObject)
	{
		if (OnInteractionFinished.IsBound())
			OnInteractionFinished.Broadcast(InteractableObject,
				ActorInteractedWithObject);
	}

	FOnInteractionFinished OnInteractionFinished;
	
};


