#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TTankPawn.h"
#include "Tanks/Interfaces/IScoreTaker.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UArrowComponent;
class ATGun;


UCLASS()
class TANKS_API ATPlayerPawn : public ATTankPawn, public IIScoreTaker
{
	GENERATED_BODY()

public:
	ATPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	//= Begin IScoreTaker interface
	void TakeScore(float Score) override;
	//= End IScoreTaker interface

protected:
	virtual void BeginPlay() override;
	virtual void ChangeGun(TSubclassOf<ATGun> GunClass) override;
	void CalculateTurretRotation();

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,	Category = "Components")
		UCameraComponent* CameraComponent;

private:
	void ShowScore() const;

private:
	float TotalScore = 0.f;

};
