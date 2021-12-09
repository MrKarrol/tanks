// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TMiniMapHolder.generated.h"

class STMiniMapWidget;

/**
 * 
 */
UCLASS()
class TANKS_API UTMiniMapHolder : public UUserWidget
{
	GENERATED_BODY()

public:

	//~ Begin UVisual Interface
	virtual void ReleaseSlateResources(bool bReleaseChildren) override;
	//~ End UVisual Interface
	float GetMiniMapSize() const;

	void SetBoundsToPaint(float sizeX, float sizeY, TArray<TArray<FVector2D>> &&points_to_bound, TArray<FVector2D> && player_points);

protected:
	//~ Begin UWidget Interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	//~ End UWidget Interface

protected:
	TSharedPtr<STMiniMapWidget> SlateMiniMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MiniMapSize;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MiniMapThickness;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MiniMapBuffer;
	
};