// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ComboBoxString.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UCheckBox;
class UVerticalBox;

/**
 * 
 */
UCLASS()
class TANKS_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

protected:
	UFUNCTION()
	void OnNewGameClicked();

	UFUNCTION()
	void OnOptionsClicked();
	
	UFUNCTION()
	void OnQuitGameClicked();

	UFUNCTION()
	void OnIsImmortalCheckBoxStateChanged(bool is_checked);

	UFUNCTION()
	void OnResolutionComboBoxOptionChosen(FString SelectedItem, ESelectInfo::Type SelectionType);

	UFUNCTION()
	void OnReturnToMainMenuClicked();

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox *MainMenu;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UVerticalBox *Options;
	
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *NewGameBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *OptionsBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *QuitGameBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UCheckBox *IsImmortalCheckBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UComboBoxString *ResolutionComboBox;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UButton *ReturnToMainMenuBtn;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidgetAnim), Transient)
	UWidgetAnimation * NewGameAnimation;

	UPROPERTY(BlueprintReadWrite)
	TSoftObjectPtr<UWorld> LevelToStart;
	
};
