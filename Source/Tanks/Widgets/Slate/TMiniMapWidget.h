// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widgets/SLeafWidget.h"

/**
 * 
 */
class TANKS_API STMiniMapWidget : public SLeafWidget
{
public:
	SLATE_BEGIN_ARGS(STMiniMapWidget)
		{
		
		}
	SLATE_ARGUMENT( float, Size )
	SLATE_ARGUMENT( float, Thickness )
	SLATE_ARGUMENT( float, Buffer )
	SLATE_END_ARGS()

	/** Constructs this widget with InArgs */
	void Construct(const FArguments& InArgs);
	
	void SetBoundsToPaint(float sizeX, float sizeY, TArray<TArray<FVector2D>> &&points_to_bound, TArray<FVector2D> && _player_points);
	float GetMiniMapSize() const;
	
private:
	virtual int32 OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
		FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
		bool bParentEnabled) const override;

	virtual FVector2D ComputeDesiredSize(float) const override;

private:
	float Size = 10.f;
	float Thickness = 1.f;
	float Buffer = 2.f;
	float SizeX = 0.f;
	float SizeY = 0.f;
	TArray<TArray<FVector2D>> BoundsToPaint;
	TArray<FVector2D> PlayerPoints;
	mutable float ScreenWidth;
	mutable float ScreenHeight;
	
};
