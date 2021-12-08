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
	return MiniMapSize;
}

void UTMiniMapHolder::SetBoundsToPaint(float _sizeX, float _sizeY, TArray<TArray<FVector2D>>&& _bounds_to_paint, TArray<FVector2D> && _player_points)
{
	sizeX = _sizeX;
	sizeY = _sizeY;
	bounds_to_paint = std::move(_bounds_to_paint);
	player_points = std::move(_player_points);
	ReleaseSlateResources(true);
	RebuildWidget();
}

TSharedRef<SWidget> UTMiniMapHolder::RebuildWidget()
{
	SlateMiniMap = SNew(STMiniMapWidget)
		.Size(MiniMapSize)
		.Thickness(MiniMapThickness)
		.Buffer(MiniMapBuffer)
		.SizeX(sizeX)
		.SizeY(sizeY)
		.BoundsToPaint(bounds_to_paint)
		.PlayerPoints(player_points);

	return SlateMiniMap.ToSharedRef();
}
