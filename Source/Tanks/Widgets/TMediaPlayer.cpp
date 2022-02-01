// Fill out your copyright notice in the Description page of Project Settings.


#include "TMediaPlayer.h"

#include "DesktopPlatformModule.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/KismetRenderingLibrary.h"


void UTMediaPlayer::NativeConstruct()
{
	Super::NativeConstruct();

	if (Browse)
		Browse->OnPressed.AddDynamic(this, &UTMediaPlayer::OnBrowseClicked);
}

void UTMediaPlayer::OnBrowseClicked()
{
	TArray<FString> OutFileNames;
	IDesktopPlatform* DesktopPlatform =
		FDesktopPlatformModule::Get();

	FString Path;
	
	if (DesktopPlatform)
	{
		FString FileTypes = "All Files(*.PNG;*.JPG;*.MP4)|*.PNG;*.JPG;*.MP4|"
						"Image Files(*.PNG;*.JPG;)|*.PNG;*.JPG;|"
						"Video Files(*.MP4)|*.MP4";
		uint32 SelectionFlag = 0;
		DesktopPlatform->OpenFileDialog(
			FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr),
			"Choose content", "", "", FileTypes, SelectionFlag, OutFileNames);

		if (OutFileNames.Num() > 0)
		{
			Path = OutFileNames[0];
		}
	}

	if (! Path.IsEmpty())
	{
		UTexture2D * Texture = UKismetRenderingLibrary::ImportFileAsTexture2D(this, 
		Path);

		if (Texture && Preview)
		{
			Preview->SetBrushFromTexture(Texture);
		}
	}
}
