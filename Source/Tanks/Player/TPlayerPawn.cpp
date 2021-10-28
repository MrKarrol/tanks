#include "TPlayerPawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"


ATPlayerPawn::ATPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BodyMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("BodyMeshComponent");
	SetRootComponent(BodyMeshComponent);

	TurretMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("TurretMeshComponent");
	TurretMeshComponent->SetupAttachment(BodyMeshComponent);

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(BodyMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATPlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
