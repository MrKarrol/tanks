#include "TTraceGun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Interfaces/IDamageTaker.h"


void ATTraceGun::DoFire()
{
	Super::DoFire();

	if (CanFire())
	{
		FVector start_trace = FirePointComponent->GetComponentLocation();
		FVector end_trace = start_trace + FirePointComponent->GetForwardVector() * TraceDistance;
		EDrawDebugTrace::Type drawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		FHitResult result;
		TArray <AActor*> ignore_list;
		GetOwner()->GetAllChildActors(ignore_list, true);
		ignore_list.Add(GetOwner());
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), start_trace, end_trace, TraceChannel, false, ignore_list, drawDebugType, result, true);
		if (result.GetActor())
			ProceedDamage(result.GetActor());
	}
}
