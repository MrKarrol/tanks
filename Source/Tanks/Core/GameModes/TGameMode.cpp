#include "TGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


void ATGameMode::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetFirstPlayerController()->SetShowMouseCursor(true);
}

void ATGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	int32 current_enemies_count = 0;
	for (const auto enemy_class : EnemiesClasses)
	{
		if (!enemy_class)
			continue;
		TArray<AActor*> out_actors;;
		UGameplayStatics::GetAllActorsOfClass(this, enemy_class, out_actors);
		current_enemies_count += out_actors.Num();
	}
	OnEnemiesNumberChangedDelegate.Broadcast(current_enemies_count);
}
