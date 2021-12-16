// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TInventoryWidget.generated.h"

struct FTInventorySlotInfo;
struct FTInventoryItemInfo;

class UUniformGridPanel;

class UTInventoryCellWidget;

/**
 * 
 */
UCLASS()
class TANKS_API UTInventoryWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void Init(int32 ItemsNum);
	bool AddItem(const FTInventorySlotInfo &, const FTInventoryItemInfo &, int32 SlotPosition = -1);

protected:
	UTInventoryCellWidget * CreateCellWidget();

protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel * ItemCellsGrid;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTInventoryCellWidget> CellWidgetClass;

	UPROPERTY()
	TArray<UTInventoryCellWidget *> CellWidgets;

	UPROPERTY(meta = (BindWidget))
	UTInventoryCellWidget * PiastreCell;

};
