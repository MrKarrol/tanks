#include "TTraceGun.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"
#include "Tanks/Interfaces/IDamageTaker.h"


void ATTraceGun::DoFire()
{
	if (CanFire())
	{
		Super::DoFire();

		FVector start_trace = FirePointComponent->GetComponentLocation();
		FVector end_trace = start_trace + FirePointComponent->GetForwardVector() * TraceDistance;
		EDrawDebugTrace::Type drawDebugType = bDrawDebugTrace ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		FHitResult result;
		UKismetSystemLibrary::LineTraceSingle(GetWorld(), start_trace, end_trace, TraceChannel, false, {}, drawDebugType, result, true);
		if (auto damage_taker = Cast<IIDamageTaker>(result.GetActor()))
		{
			FTDamageData data;
			data.Damage = Damage;
			data.Instigator = GetOwner();
			damage_taker->TakeDamage(data);
		}
	}
}
