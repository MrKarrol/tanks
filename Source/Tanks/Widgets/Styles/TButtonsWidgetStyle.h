// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/RichTextBlock.h"
#include "Styling/SlateWidgetStyle.h"
#include "Styling/SlateWidgetStyleContainerBase.h"

#include "TButtonsWidgetStyle.generated.h"

/**
 * 
 */
USTRUCT()
struct TANKS_API FTButtonStyle : public FSlateWidgetStyle
{
	GENERATED_USTRUCT_BODY()

	FTButtonStyle();
	virtual ~FTButtonStyle();

	// FSlateWidgetStyle
	virtual void GetResources(TArray<const FSlateBrush*>& OutBrushes) const override;
	static const FName TypeName;
	virtual const FName GetTypeName() const override { return TypeName; };
	static const FTButtonStyle& GetDefault();

	UPROPERTY(EditAnywhere, Category = Appearance)
	FButtonStyle ButtonStyle;
};

/**
 */
UCLASS(hidecategories=Object, MinimalAPI)
class UTButtonWidgetStyle : public USlateWidgetStyleContainerBase
{
	GENERATED_BODY()

public:
	/** The actual data describing the widget appearance. */
	UPROPERTY(Category=Appearance, EditAnywhere, meta=(ShowOnlyInnerProperties))
	FTButtonStyle WidgetStyle;

	virtual const struct FSlateWidgetStyle* const GetStyle() const override
	{
		return static_cast<const struct FSlateWidgetStyle*>(&WidgetStyle);
	}
};
