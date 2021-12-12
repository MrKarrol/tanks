// Fill out your copyright notice in the Description page of Project Settings.


#include "TButtonsWidgetStyle.h"


FTButtonStyle::FTButtonStyle()
{
	ButtonStyle = FCoreStyle::Get().GetWidgetStyle<FButtonStyle>("Button");
}

FTButtonStyle::~FTButtonStyle()
{
}

const FName FTButtonStyle::TypeName(TEXT("FButtoStyle"));

const FTButtonStyle& FTButtonStyle::GetDefault()
{
	static FTButtonStyle Default;
	return Default;
}

void FTButtonStyle::GetResources(TArray<const FSlateBrush*>& OutBrushes) const
{
	// Add any brush resources here so that Slate can correctly atlas and reference them
}
