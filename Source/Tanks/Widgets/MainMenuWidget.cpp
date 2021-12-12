#include "MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"

#include "Kismet/GameplayStatics.h"
#include "Styles/TButtonsWidgetStyle.h"
#include "Styles/StyleSet.h"

void UMainMenuWidget::NativePreConstruct()
{
	if (NewGameBtn)
		NewGameBtn->WidgetStyle = 
			FStyleSet::Get().GetWidgetStyle<FTButtonStyle>(
				FName("BP_ButtonWidgetStyle")
			).ButtonStyle;

	if (OptionsBtn)
		OptionsBtn->WidgetStyle = 
			FStyleSet::Get().GetWidgetStyle<FTButtonStyle>(
				FName("BP_ButtonWidgetStyle")
			).ButtonStyle;

	if (QuitGameBtn)
		QuitGameBtn->WidgetStyle = 
			FStyleSet::Get().GetWidgetStyle<FTButtonStyle>(
				FName("BP_ButtonWidgetStyle")
			).ButtonStyle;

	if (ReturnToMainMenuBtn)
		ReturnToMainMenuBtn->WidgetStyle = 
			FStyleSet::Get().GetWidgetStyle<FTButtonStyle>(
				FName("BP_ButtonWidgetStyle")
			).ButtonStyle;
}

void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (NewGameBtn)
		NewGameBtn->OnPressed.AddDynamic(this, &UMainMenuWidget::OnNewGameClicked);

	if (OptionsBtn)
		OptionsBtn->OnPressed.AddDynamic(this, &UMainMenuWidget::OnOptionsClicked);

	if (QuitGameBtn)
		QuitGameBtn->OnPressed.AddDynamic(this, &UMainMenuWidget::OnQuitGameClicked);

	if(IsImmortalCheckBox)
		IsImmortalCheckBox->OnCheckStateChanged.AddDynamic(this, &UMainMenuWidget::OnIsImmortalCheckBoxStateChanged);

	if(ResolutionComboBox)
		ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &UMainMenuWidget::OnResolutionComboBoxOptionChosen);

	if (ReturnToMainMenuBtn)
		ReturnToMainMenuBtn->OnClicked.AddDynamic(this, &UMainMenuWidget::OnReturnToMainMenuClicked);

	if (NewGameAnimation)
		PlayAnimation(NewGameAnimation);
}

void UMainMenuWidget::OnNewGameClicked()
{
	UGameplayStatics::OpenLevel(this, *LevelToStart.GetAssetName());
}

void UMainMenuWidget::OnOptionsClicked()
{
	MainMenu->SetVisibility(ESlateVisibility::Hidden);
	Options->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::OnQuitGameClicked()
{
	UKismetSystemLibrary::QuitGame(this, GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, false);
}

void UMainMenuWidget::OnIsImmortalCheckBoxStateChanged(bool is_checked)
{
	// save parameter
}

void UMainMenuWidget::OnResolutionComboBoxOptionChosen(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if (SelectionType == ESelectInfo::OnMouseClick)
		UKismetSystemLibrary::ExecuteConsoleCommand(this, TEXT("r.setRes ") + SelectedItem, GetWorld()->GetFirstPlayerController());
}

void UMainMenuWidget::OnReturnToMainMenuClicked()
{
	Options->SetVisibility(ESlateVisibility::Hidden);
	MainMenu->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::SetButtonStyle(const FTButtonStyle* InStyle)
{
	NewGameBtn->SetStyle(InStyle->ButtonStyle);
	OptionsBtn->SetStyle(InStyle->ButtonStyle);	
	QuitGameBtn->SetStyle(InStyle->ButtonStyle);
	ReturnToMainMenuBtn->SetStyle(InStyle->ButtonStyle);
}
