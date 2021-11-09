#pragma once

#include "GameFramework/Actor.h"

#include "TAmmoBox.generated.h"


class ATGun;
class UBoxComponent;

UCLASS()
class ATAmmoBox : public AActor
{
public:
	GENERATED_BODY()

public:
	ATAmmoBox();

public:
	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* MeshComponent;

protected:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};