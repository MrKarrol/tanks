#include "TMapPortal.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tanks/Player/TPlayerPawn.h"


ATMapPortal::ATMapPortal()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent");
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ATMapPortal::OnBeginOverlap);
	SetRootComponent(BoxComponent);
}

void ATMapPortal::BeginPlay()
{
	Super::BeginPlay();

	SetPortalEnabled(false);
}

void ATMapPortal::SetPortalEnabled(bool bEnabled)
{
	bPortalEnabled = bEnabled;

	OnPortalEnabledChanged();
}

void ATMapPortal::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bPortalEnabled)
		return;

	if (auto pawn = Cast<ATPlayerPawn>(OtherActor))
	{
		LoadMap();
	}
}

void ATMapPortal::LoadMap()
{
	UGameplayStatics::OpenLevel(this, MapName);
}
