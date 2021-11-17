#pragma once

#include "GameFramework/Actor.h"
#include "TMapPortal.generated.h"

class UBoxComponent;

UCLASS()
class ATMapPortal : public AActor
{
	GENERATED_BODY()
public:
	ATMapPortal();
	void BeginPlay() override;

	void SetPortalEnabled(bool bEnabled);

protected:
	UFUNCTION(BlueprintImplementableEvent)
		void OnPortalEnabledChanged();

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent,
							AActor* OtherActor,
							UPrimitiveComponent* OtherComp,
							int32 OtherBodyIndex, 
							bool bFromSweep, 
							const FHitResult& SweepResult);

	void LoadMap();
	void OnEndFactoryBattle();

public:
	UPROPERTY(EditAnywhere)
		UBoxComponent* BoxComponent;

protected:
	UPROPERTY(EditAnywhere)
		TSoftObjectPtr<UWorld> WorldToLoad;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bPortalEnabled = false;

};