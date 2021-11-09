#pragma once

#include "GameFramework/Actor.h"

#include "TGunBox.generated.h"


class ATGun;
class UBoxComponent;

UCLASS()
class ATGunBox : public AActor
{
public:
	GENERATED_BODY()

public:
	ATGunBox();
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		TSubclassOf<ATGun> GunClass;

protected:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};