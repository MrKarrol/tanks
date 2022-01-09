// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TWidget.generated.h"

struct FTButtonStyle;

/**
 * 
 */
UCLASS()
class TANKS_API UTWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE(FOnNeedToRemove);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOwningActorChanged, AActor*, NewOwner);

public:

	/* Set the owning actor so widgets have access to whatever is, broadcasting OnOwningActorChanged event */
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetOwningActor(AActor* NewOwner);

	UPROPERTY(BlueprintAssignable, Category = "UI")
	FOnOwningActorChanged OnOwningActorChanged;

	virtual void SetButtonsStyle(const FTButtonStyle* InStyle);
	
public:
	FOnNeedToRemove OnNeedToRemove;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "ActorWidget")
	AActor* OwningActor;
	
};
