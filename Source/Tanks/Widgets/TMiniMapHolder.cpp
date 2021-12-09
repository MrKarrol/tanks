// Fill out your copyright notice in the Description page of Project Settings.


#include "TMiniMapHolder.h"

#include "Tanks/Widgets/Slate/TMiniMapWidget.h"


void UTMiniMapHolder::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	SlateMiniMap.Reset();
}

float UTMiniMapHolder::GetMiniMapSize() const
{
	return SlateMiniMap->GetMiniMapSize();
}

void UTMiniMapHolder::SetBoundsToPaint(float size_x, float size_y, TArray<TArray<FVector2D>>&& bounds_to_paint, TArray<FVector2D> && player_points)
{
	SlateMiniMap->SetBoundsToPaint(size_x, size_y, std::move(bounds_to_paint), std::move(player_points));
}

TSharedRef<SWidget> UTMiniMapHolder::RebuildWidget()
{
	SlateMiniMap = SNew(STMiniMapWidget)
		.Size(MiniMapSize)
		.Thickness(MiniMapThickness)
		.Buffer(MiniMapBuffer)
		;

	return SlateMiniMap.ToSharedRef();
}
