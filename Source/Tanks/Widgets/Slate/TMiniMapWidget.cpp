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

int32 STMiniMapWidget::OnPaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
    const float AllottedWidth = AllottedGeometry.GetLocalSize().X;
    const float AllottedHeight = AllottedGeometry.GetLocalSize().Y;

	const int32 ActualMapSide = AllottedHeight * FMath::Clamp(Size, 0.f, 40.f) / 100;
	const int32 ActualBufferSide = AllottedHeight * FMath::Clamp(Buffer, 0.f, 40.f) / 100;
	const int32 ActualThickness = AllottedHeight * FMath::Clamp(Thickness, 0.f, 40.f) / 100;
	
    constexpr ESlateDrawEffect DrawEffects = ESlateDrawEffect::None;

	++LayerId;

    FSlateBrush brush;
	const TArray<FVector2D> Points = {
		{AllottedWidth - ActualMapSide - ActualBufferSide, AllottedHeight - ActualMapSide - ActualBufferSide},
		{AllottedWidth - ActualBufferSide, AllottedHeight - ActualMapSide - ActualBufferSide},
		{AllottedWidth - ActualBufferSide, AllottedHeight - ActualBufferSide},
		{AllottedWidth - ActualMapSide - ActualBufferSide, AllottedHeight - ActualBufferSide},
		{AllottedWidth - ActualMapSide - ActualBufferSide, AllottedHeight - ActualMapSide - ActualBufferSide}};
    FSlateDrawElement::MakeLines(
        OutDrawElements,
        LayerId,
        AllottedGeometry.ToPaintGeometry(),
        Points,
        DrawEffects,
        FLinearColor::Red * 
            InWidgetStyle.GetColorAndOpacityTint(),
            true,
            ActualThickness
        );

    

    return LayerId;
}

FVector2D STMiniMapWidget::ComputeDesiredSize(float) const
{
	return FVector2D(Size, Size);
}

END_SLATE_FUNCTION_BUILD_OPTIMIZATION
