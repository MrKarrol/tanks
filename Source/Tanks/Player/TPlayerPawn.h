#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TTankPawn.h"
#include "Tanks/Enemies/TEnemyTurret.h"
#include "Tanks/Interfaces/IScoreTaker.h"
#include "Tanks/Interfaces/IHealable.h"
#include "InteractableObject.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
class ATGun;
class UTInventoryComponent;
class UTInventoryManagerComponent;
class UTEquipInventoryComponent;

class UInteractionComponent;
class UQuestDialog;
class UQuestList;
class UQuestListComponent;

UCLASS()
class TANKS_API ATPlayerPawn
	: public ATTankPawn
	, public IIScoreTaker
	, public IIHealable
	, public IInteractableObject
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnChangeScoreDelegate, int32 /*score*/);
public:
	ATPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetGun(TSubclassOf<ATGun> GunClass) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void MoveTurretUp(float AxisValue);
	void MoveTurretRight(float AxisValue);

	//= Begin IScoreTaker interface
	virtual void TakeScore(float Score) override;
	//= End IScoreTaker interface

	FOnChangeScoreDelegate OnChangeScoreDelegate;

	//= Begin IHealable interface
	virtual void Heal(float Health) override;
	//= End IHealable interface

	virtual void SwapGuns() override;

	void AddTurretHelper(ATEnemyTurret *);
	bool CanSpawnTurretHelper() const;
	size_t TurretHelpersNumber() const;

	void ShowInventory();
	void Interact();

protected:
	virtual void BeginPlay() override;
	
	void CalculateTopDownTurretRotation();
	void CalculateThirdViewTurretRotation(float DeltaTime);
	void OnShot(ATGun*) const;
	virtual void OnDie() override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnPawnDied();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,	Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	USpringArmComponent* SpringArmThirdViewComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UCameraComponent* CameraThirdViewComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTInventoryComponent * InventoryComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTEquipInventoryComponent * EquipmentInventoryComponent;
	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UTInventoryManagerComponent * InventoryManagerComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Helpers")
	uint8 MaxTurretHelpersCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QuestSystem")
	UInteractionComponent * InteractionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "QuestSystem")
	TSubclassOf<UQuestDialog> QuestDialogClass;

	UPROPERTY()
	UQuestList * QuestList;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QuestSystem")
	TSubclassOf<UQuestList> QuestListClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "QuestSystem")
	UQuestListComponent * QuestListComponent;

public:
	virtual void Interact_Implementation(AActor * ActorInteractedWithObject) override;

	UFUNCTION(BlueprintCallable)
	void ToggleQuestListVisibility();
	
private:
	void DefineCameraView(ATGun*);

protected:
	float mCurrentTurretUpSpeed = 0.f;
	float mCurrentTurretRotationSpeed = 0.f;

private:
	float TotalScore = 0.f;
	bool bInThirdPersonView = false;

	float mCachedTurretUpDelta;
	float mCachedTurretRotationDelta;

	TArray<ATEnemyTurret *> TurretHelpers;

};
