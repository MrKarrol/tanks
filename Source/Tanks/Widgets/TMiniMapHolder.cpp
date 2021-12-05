// Fill out your copyright notice in the Description page of Project Settings.


#include "TMiniMapHolder.h"

#include "Tanks/Widgets/Slate/TMiniMapWidget.h"


void UTMiniMapHolder::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	SlateMiniMap.Reset();
}

TSharedRef<SWidget> UTMiniMapHolder::RebuildWidget()
{
	SlateMiniMap = SNew(STMiniMapWidget)
		.Size(MiniMapSize)
		.Thickness(MiniMapThickness)
		.Buffer(MiniMapBuffer);

	return SlateMiniMap.ToSharedRef();
}