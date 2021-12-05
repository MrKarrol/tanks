#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TTankPawn.h"
#include "Tanks/Interfaces/IScoreTaker.h"
#include "Tanks/Interfaces/IHealable.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
class ATGun;


UCLASS()
class TANKS_API ATPlayerPawn : public ATTankPawn, public IIScoreTaker, public IIHealable
{
	GENERATED_BODY()

	DECLARE_DELEGATE_OneParam(FOnChangeScoreDelegate, int32 /*score*/);
public:
	ATPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
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

protected:
	virtual void BeginPlay() override;
	virtual void SwapGuns() override;
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

};
