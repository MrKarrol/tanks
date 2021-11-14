#pragma once

#include "GameFramework/Actor.h"
#include "TPatrolPoint.generated.h"

class UBillboardComponent;


UCLASS()
class ATPatrolPoint : public AActor
{
	GENERATED_BODY()
public:
	ATPatrolPoint();

public:
	UPROPERTY(EditAnywhere)
		UBillboardComponent* BillboardComponent;

};
