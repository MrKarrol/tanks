#pragma once

#include "Tanks/Core/TPawn.h"
#include "Tanks/Interfaces/IDamageTaker.h"
#include "TEnemyTurret.generated.h"

class UArrowComponent;

UCLASS()
class TANKS_API ATEnemyTurret : public ATPawn, public IIDamageTaker
{
	GENERATED_BODY()
public:
	ATEnemyTurret();
	void Tick(float DeltaTime) override;

	//= Begin IDamageTaker interface
	void TakeDamage(const FTDamageData&) override;
	//= End IDamageTaker interface

protected:
	AActor* GetTarget() const;
	bool TargetIsVisible() const;
	void ProcessTargeting(float DeltaTime);

	USceneComponent* GetGunPivotAttach() const override;
	void OnDie() override;

public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* GunPivotLocation;

};