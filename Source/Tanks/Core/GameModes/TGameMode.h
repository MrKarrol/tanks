#pragma once

#include "GameFramework/GameMode.h"
#include "TGameMode.generated.h"

class UUserWidget;

UCLASS()
class TANKS_API ATGameMode : public AGameMode
{
	GENERATED_BODY()
	DECLARE_MULTICAST_DELEGATE_OneParam(FEnemiesNumberChangedDelegate, int32 /*enemies number*/);
	
public:
	virtual void Tick(float DeltaSeconds) override;

	FEnemiesNumberChangedDelegate OnEnemiesNumberChangedDelegate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Enemies")
	TArray<TSubclassOf<AActor>> EnemiesClasses; 
	
protected:
	virtual void BeginPlay() override;

};
