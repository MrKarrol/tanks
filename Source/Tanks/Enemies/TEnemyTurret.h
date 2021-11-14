#pragma once

#include "Tanks/Core/TPawn.h"
#include "Tanks/Interfaces/IDamageTaker.h"
#include "Tanks/Interfaces/IScorable.h"
#include "TEnemyTurret.generated.h"

class UArrowComponent;

UCLASS()
class TANKS_API ATEnemyTurret : public ATPawn, public IIDamageTaker, public IIScorable
{
	GENERATED_BODY()
public:
	ATEnemyTurret();
	void Tick(float DeltaTime) override;

	//= Begin IDamageTaker interface
	void TakeDamage(const FTDamageData&) override;
	bool IsDead() const override;
	//= End IDamageTaker interface

	//= Begin IScorable interface
	virtual float GetScore() override;
	//= End IScorable interface

protected:
	AActor* GetTarget() const;
	bool TargetIsVisible() const;
	void ProcessTargeting(float DeltaTime);

	USceneComponent* GetGunPivotAttach() const override;
	void OnDie() override;


public:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
		UArrowComponent* GunPivotLocation;

	UPROPERTY(EditAnywhere, Category = "Score")
		float Score = 25.f;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Fire")
		TEnumAsByte<ETraceTypeQuery> TraceChannel;

};