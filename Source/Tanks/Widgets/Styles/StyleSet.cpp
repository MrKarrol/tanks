﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "StyleSet.h"

#include "Engine.h"
#include "Slate/SlateGameResources.h"
#include "Styling/ISlateStyle.h"


TSharedPtr<ISlateStyle> FindStyle(const FString& ScopeToDirectory)
{
	TSharedPtr<ISlateStyle> Style = FSlateGameResources::New(FName("StyleSet"), 
		ScopeToDirectory, ScopeToDirectory);

	return Style;
}

TSharedPtr<ISlateStyle> FStyleSet::StylePtr = NULL;

void FStyleSet::Initialize()
{
	Shutdown();

	StylePtr = FindStyle("/Game/Tanks/Widgets/Styles");
	FSlateStyleRegistry::RegisterSlateStyle(*StylePtr);
}

void FStyleSet::Shutdown()
{
	if (StylePtr.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StylePtr);
		ensure(StylePtr.IsUnique());
		StylePtr.Reset();
	}
}

const ISlateStyle& FStyleSet::Get()
{
	if (!StylePtr.IsValid())
	{
		Initialize();
	}    
	return *StylePtr;
}
