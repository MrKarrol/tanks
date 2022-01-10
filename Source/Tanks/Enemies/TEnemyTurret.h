#pragma once

#include "Tanks/Core/TPawn.h"
#include "TEnemyTurret.generated.h"

class UArrowComponent;
class ATGun;

UCLASS()
class TANKS_API ATEnemyTurret : public ATPawn
{
	GENERATED_BODY()
public:
	ATEnemyTurret();
	void Tick(float DeltaTime) override;
	void SetInitialGun(TSubclassOf<ATGun> InitialGun);

protected:
	AActor* GetTarget() const;
	bool TargetIsVisible() const;
	void ProcessTargeting(float DeltaTime);

	USceneComponent* GetGunPivotAttach() const override;
	void OnDie() override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* GunPivotLocation;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

};