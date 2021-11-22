#pragma once

#include "GameFramework/Actor.h"

#include "THealthBox.generated.h"


class UBoxComponent;

UCLASS()
class ATHealthBox : public AActor
{
public:
	GENERATED_BODY()

public:
	ATHealthBox();

public:
	UPROPERTY(EditAnywhere, Category = "Components")
		UBoxComponent* BoxComponent;

	UPROPERTY(EditAnywhere, Category = "Components")
		USkeletalMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, Category = "Heal")
		float HealthGain = 500.f;

protected:
	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};