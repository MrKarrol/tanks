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
	void Tick(float DeltaTime) override;
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void SetGun(TSubclassOf<ATGun> GunClass) override;

	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);

	//= Begin IScoreTaker interface
	void TakeScore(float Score) override;
	//= End IScoreTaker interface

protected:
	void BeginPlay() override;
	void ChangeGun(TSubclassOf<ATGun> GunClass) override;
	void CalculateTurretRotation();
	void OnShot(ATGun*);

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

private:
	void ShowScore() const;

private:
	float TotalScore = 0.f;

};
