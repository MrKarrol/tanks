#pragma once

#include "GameFramework/GameMode.h"
#include "TGameMode.generated.h"

class UUserWidget;

UCLASS()
class TANKS_API ATGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	ATGameMode();
	virtual ~ATGameMode() override;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ChangeMenuWidget(TSubclassOf<UUserWidget> NewWidgetClass);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TSubclassOf<UUserWidget> StartingWidgetClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

};
