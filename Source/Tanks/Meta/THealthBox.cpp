#include "THealthBox.h"

#include "Components/BoxComponent.h"
#include "Tanks/Tanks.h"
#include "Tanks/Interfaces/IHealable.h"


ATHealthBox::ATHealthBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATHealthBox::OnBeginOverlap);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(BoxComponent);
}

void ATHealthBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (auto healable = Cast<IIHealable>(OtherActor))
	{
		healable->Heal(HealthGain);
	}
}
