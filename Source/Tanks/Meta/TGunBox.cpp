#include "TGunBox.h"

#include "Components/BoxComponent.h"
#include "Tanks/Guns/TGun.h"
#include "Tanks/Player/TPlayerPawn.h"
#include "Tanks/Tanks.h"


ATGunBox::ATGunBox()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->SetupAttachment(RootComponent);

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATGunBox::OnBeginOverlap);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(BoxComponent);
}

void ATGunBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATGunBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATPlayerPawn* player_pawn = Cast<ATPlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player_pawn == OtherActor)
	{
		player_pawn->SetGun(GunClass);
	}
}
