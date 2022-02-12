// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Objective.generated.h"

UENUM()
enum class EObjectiveType : uint8
{
	Location,
	Interact,
	Collect,
	Kill
};

/**
 * 
 */
UCLASS()
class QUESTSYSTEM_API UObjective : public UObject
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnObjectiveCompleted, UObjective * /*Objective*/);

public:
	virtual void ActivateObjective(AActor * Character) {}

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(VisibleAnywhere)
	EObjectiveType Type;

	FOnObjectiveCompleted OnObjectiveCompleted;

	UPROPERTY(VisibleAnywhere)
	bool bIsCompleted;

	UPROPERTY(VisibleAnywhere)
	bool bCanBeCompleted;
	
};


UCLASS()
class QUESTSYSTEM_API UInteractionObjective : public UObjective
{
	GENERATED_BODY()
	
public:
	UInteractionObjective();

	virtual void ActivateObjective(AActor* Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "InteractableObject"))
	AActor * Target;
	
};

UCLASS()
class QUESTSYSTEM_API ULocationObjective : public UObjective
{
	GENERATED_BODY()

public:
	ULocationObjective();

	virtual void ActivateObjective(AActor* Character) override;

	UPROPERTY(EditAnywhere, meta = (AllowedClasses = "LocationMarker"))
	AActor * Marker;
	
};