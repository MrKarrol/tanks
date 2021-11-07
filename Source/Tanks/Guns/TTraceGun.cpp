#include "TTraceGun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"


void ATTraceGun::DoFire()
{
	FVector start_trace = FirePointComponent->GetComponentLocation();
	FVector end_trace = start_trace + FirePointComponent->GetForwardVector() * TraceDistance;
	EDrawDebugTrace::Type drawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
	FHitResult result;
	UKismetSystemLibrary::LineTraceSingle(GetWorld(), start_trace, end_trace, TraceChannel, false, {}, drawDebugType, result, true);
}
