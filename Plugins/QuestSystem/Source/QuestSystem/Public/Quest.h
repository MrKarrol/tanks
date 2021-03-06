// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Quest.generated.h"

class UObjective;

UCLASS()
class QUESTSYSTEM_API AQuest : public AActor
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_OneParam(FOnQuestStatusUpdated, AQuest *);
	
public:
	// Sets default values for this actor's properties
	AQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void UpdateLocation();

	void TakeQuest(AActor * Character);

	FOnQuestStatusUpdated OnQuestStatusUpdated;

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddLocationObjective();

	UFUNCTION(BlueprintCallable, CallInEditor)
	void AddInteractObjective();

	TArray<UObjective*> GetObjectives() const;

	const FText & GetName() const;

	const FText & GetDescription() const;

	bool IsAlreadyTaken() const noexcept;

	bool IsStoryQuest() const noexcept;

	AQuest * GetPrerequisiteQuest() const;

	bool IsCompleted() const noexcept;

protected:
	void OnObjectiveCompleted(UObjective * Objective);

protected:
	UPROPERTY(EditAnywhere)
	FText Name;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	TArray<UObjective *> Objectives;
	
	UPROPERTY(EditAnywhere)
	bool bIsStoryQuest = true;

	UPROPERTY(EditAnywhere)
	bool bKeepObjectivesOrder = true;

	UPROPERTY(EditAnywhere)
	AQuest * PrerequisiteQuest;

	UPROPERTY(EditAnywhere)
	bool bIsTaken;

	UPROPERTY()
	bool bIsCompleted;
	
};
