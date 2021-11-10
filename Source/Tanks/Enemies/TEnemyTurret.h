#pragma once

#include "Tanks/Core/TPawn.h"
#include "TEnemyTurret.generated.h"

class UArrowComponent;

UCLASS()
class TANKS_API ATEnemyTurret : public ATPawn
{
	GENERATED_BODY()
public:
	ATEnemyTurret();
	void Tick(float DeltaTime) override;

protected:
	AActor* GetTarget() const;
	bool TargetIsVisible() const;
	void ProcessTargeting(float DeltaTime);

	USceneComponent* GetGunPivotAttach() const override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* GunPivotLocation;

	UPROPERTY(EditAnywhere)
		float FireAccuracy = 2.f;

};