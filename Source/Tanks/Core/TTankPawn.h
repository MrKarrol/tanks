#pragma once

#include "CoreMinimal.h"
#include "Tanks/Core/TPawn.h"
#include "TTankPawn.generated.h"

class UArrowComponent;
class ATGun;


UCLASS()
class TANKS_API ATTankPawn : public ATPawn
{
	GENERATED_BODY()

public:
	ATTankPawn();
	void Tick(float DeltaTime) override;

	void AddTankMovementInput(float MoveDelta);
	void AddTankRotationInput(float RotationDelta);
	void AddTankTurretRotationInput(FRotator RotationDelta);

	USceneComponent* GetGunPivotAttach() const override;

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* GunPivotLocation;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveSpeed = 512.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float MoveAcceleration = 1024.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationAcceleration = 64.f;

protected:
	void PerformMovement(float DeltaTime);
	void PerformTurretRotation();

protected:
	float mCurrentMoveSpeed = 0.f;
	float mCurrentRotationSpeed = 0.f;

private:
	float mCachedMoveDelta;
	float mCachedRotationDelta;
	FRotator mCachedTurretRotationDelta;

};
