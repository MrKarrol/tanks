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

public:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* BodyMeshComponent;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TurretMeshComponent;

	UPROPERTY(VisibleAnywhere)
		USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* CameraComponent;

protected:
	virtual void BeginPlay() override;

private:

};
