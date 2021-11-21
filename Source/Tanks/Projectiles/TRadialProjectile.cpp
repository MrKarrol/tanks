#include "TRadialProjectile.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/AudioComponent.h"


ATRadialProjectile::ATRadialProjectile()
{
	DieFXComponent = CreateDefaultSubobject<UParticleSystemComponent>("DieFXComponent");
	DieFXComponent->SetupAttachment(Mesh);

	DieAudioComponent = CreateDefaultSubobject<UAudioComponent>("DieAudioComponent");
	DieAudioComponent->SetupAttachment(Mesh);
	DieAudioComponent->bStopWhenOwnerDestroyed = false;
}

void ATRadialProjectile::OnBounce(const FHitResult& ImpactResult, const FVector& ImpactVelocity)
{
	const FVector trace_start = ImpactResult.ImpactPoint;
	const FVector trace_end = trace_start + FVector::OneVector;
	TArray<FHitResult> hits;
	UKismetSystemLibrary::SphereTraceMulti(this, trace_start, trace_end, ExplodeRadius, TraceChannel, false, {}, EDrawDebugTrace::None, hits, false);

	for (auto& hit : hits)
	{
		if (hit.GetActor() == GetOwner())
			return;

		if (OnHitSomethingDelegate.IsBound())
			OnHitSomethingDelegate.Execute(hit.GetActor());

		if (auto actor = hit.GetActor())
		{
			TArray<UPrimitiveComponent*> physics_components;
			actor->GetComponents(physics_components);
			for (auto component : physics_components)
			{
				if (component->IsSimulatingPhysics())
				{
					const FVector force_direction = actor->GetActorLocation() - GetActorLocation().Normalize();
					component->AddImpulse(force_direction * PushForce);
				}
			}
		}
	}

	DieFXComponent->ActivateSystem();
	DieAudioComponent->Play();

	Destroy();
}
