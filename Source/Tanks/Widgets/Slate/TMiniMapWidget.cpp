// Fill out your copyright notice in the Description page of Project Settings.


#include "TMiniMapWidget.h"

#include "SlateOptMacros.h"

BEGIN_SLATE_FUNCTION_BUILD_OPTIMIZATION

void STMiniMapWidget::Construct(const FArguments& InArgs)
{
	Size = InArgs._Size;
	Thickness = InArgs._Thickness;
	Buffer = InArgs._Buffer;
	/*
	ChildSlot
	[
		// Populate the widget
	];
	*/
}

void STMiniMapWidget::SetBoundsToPaint(float sizeX, float sizeY, TArray<TArray<FVector2D>>&& points_to_bound,
	TArray<FVector2D>&& _player_points)
{
	SizeX = sizeX;
	SizeY = sizeY;
	BoundsToPaint = std::move(points_to_bound);
	PlayerPoints = std::move(_player_points);
}

float STMiniMapWidget::GetMiniMapSize() const
{
	return FMath::Max(ScreenWidth, ScreenHeight) * Size / 100.f;
}

int32 STMiniMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
                               const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
                               const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const float AllottedWidth = AllottedGeometry.GetLocalSize().X;
	ScreenWidth = AllottedWidth;
    const float AllottedHeight = AllottedGeometry.GetLocalSize().Y;
	ScreenHeight = AllottedHeight;

	// const int32 ActualMapSide = AllottedHeight * FMath::Clamp(Size, 0.f, 40.f) / 100;
	const int32 ActualBufferSide = AllottedHeight * FMath::Clamp(Buffer, 0.f, 40.f) / 100;
	const int32 ActualThickness = AllottedHeight * FMath::Clamp(Thickness, 0.f, 40.f) / 100;
	
    constexpr ESlateDrawEffect DrawEffects = ESlateDrawEffect::None;

	++LayerId;

    FSlateBrush brush;
	const FLinearColor main_color = FLinearColor::Red;
	const FLinearColor player_color = FLinearColor::Yellow;
	for (const auto &point_array : BoundsToPaint)
	{
		TArray<FVector2D>  points_to_paint;
		for (const auto &point : point_array)
		{
			points_to_paint.Add({AllottedWidth - SizeX - ActualBufferSide + point.X, AllottedHeight - SizeY - ActualBufferSide + point.Y});
		}
		FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		points_to_paint,
		DrawEffects,
		main_color * 
			InWidgetStyle.GetColorAndOpacityTint(),
			true,
			ActualThickness
		);
	}
	// print player
    {
    	TArray<FVector2D>  points_to_paint;
    	for (const auto &point : PlayerPoints)
    	{
    		points_to_paint.Add({AllottedWidth - SizeX - ActualBufferSide + point.X, AllottedHeight - SizeY - ActualBufferSide + point.Y});
    	}
    	FSlateDrawElement::MakeLines(
		OutDrawElements,
		LayerId,
		AllottedGeometry.ToPaintGeometry(),
		points_to_paint,
		DrawEffects,
		player_color * 
			InWidgetStyle.GetColorAndOpacityTint(),
			true,
			ActualThickness
		);
    }
    

    return LayerId;
}

FVector2D STMiniMapWidget::ComputeDesiredSize(float) const
{
	return FVector2D(Size, Size);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
