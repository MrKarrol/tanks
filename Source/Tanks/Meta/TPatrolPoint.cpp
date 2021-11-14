#include "TPatrolPoint.h"

#include "Components/BillboardComponent.h"

ATPatrolPoint::ATPatrolPoint()
{
	BillboardComponent = CreateDefaultSubobject<UBillboardComponent>("BillboardComponent");
	SetRootComponent(BillboardComponent);
}