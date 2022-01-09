
#pragma once

#include "CoreMinimal.h"
#include "Tanks/Widgets/TWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;

UCLASS()
class TANKS_API UPauseMenuWidget : public UTWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	virtual void RemoveFromParent() override;

protected:
	UFUNCTION()
	void OnReturnToGameClicked();

	UFUNCTION()
	void OnMainMenuClicked();

	UFUNCTION()
	void OnQuitClicked();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *ReturnToGameBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *MainMenuBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *QuitBtn;

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UWorld> MainMenuLevel;
	
};
