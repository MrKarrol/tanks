﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TWidget.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_DELEGATE(FOnNeedToRemove);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningActorChanged, AActor*, NewOwner);

public:

	/* Set the owning actor so widgets have access to whatever is, broadcasting OnOwningActorChanged event */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetOwningActor(AActor* NewOwner);

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnOwningActorChanged OnOwningActorChanged;
	
public:
	FOnNeedToRemove OnNeedToRemove;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ActorWidget")
	AActor* OwningActor;
	
};
