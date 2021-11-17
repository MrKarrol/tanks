#include "TMapPortal.h"

#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tanks/Core/GameModes/TFactoryBattleGameMode.h"
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

	SetPortalEnabled(bPortalEnabled);
	if (auto game_mode = Cast<ATFactoryBattleGameMode>(GetWorld()->GetAuthGameMode()))
	{
		game_mode->OnEndFactoryBattleDelegate.AddUObject(this, &ATMapPortal::OnEndFactoryBattle);
	}
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
	UGameplayStatics::OpenLevel(this, *WorldToLoad.GetAssetName());
}

void ATMapPortal::OnEndFactoryBattle()
{
	SetPortalEnabled(true);
}
