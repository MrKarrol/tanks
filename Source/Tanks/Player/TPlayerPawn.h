#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TPlayerPawn.generated.h"

class UCameraComponent;
class USpringArmComponent;

UCLASS()
class TANKS_API ATPlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	ATPlayerPawn();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void MoveForward(float AxisValue);

	UFUNCTION()
		void MoveRight(float AxisValue);

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite,	Category = "Components")
		UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100;

	float _targetForwardAxisValue;
	float _targetRightAxisValue;

protected:
	virtual void BeginPlay() override;

private:

};
