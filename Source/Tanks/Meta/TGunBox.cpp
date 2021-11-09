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

	GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, FString::Printf(TEXT("%s"), *BoxComponent->GetComponentLocation().ToString()));
}

void ATGunBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogT, Error, TEXT("Collision"));
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Got 0")));
	ATPlayerPawn* player_pawn = Cast<ATPlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player_pawn == OtherActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("Got")));
		player_pawn->SetGun(GunClass);
	}
}
