// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TInventoryWidget.generated.h"

struct FTInventorySlotInfo;
struct FTInventoryItemInfo;

class UUniformGridPanel;
class UTInventoryComponent;
class UTInventoryCellWidget;

/**
 * 
 */
UCLASS()
class TANKS_API UTInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, 
	UTInventoryCellWidget * /*DraggedFrom*/, UTInventoryCellWidget * /*DroppedTo*/);
public:
	virtual void NativeConstruct() override;

	void Init(int32 ItemsNum);
	bool AddItem(const FTInventorySlotInfo &, const FTInventoryItemInfo &, int32 SlotPosition = -1);

protected:
	UTInventoryCellWidget * CreateCellWidget();
	
	void InitCellWidget(UTInventoryCellWidget* Widget);

	void OnItemDropped(UTInventoryCellWidget * DraggedFrom, 
	                   UTInventoryCellWidget * DroppedTo);

public:
	FOnItemDrop OnItemDrop;

	UPROPERTY()
	UTInventoryComponent * RepresentedInventory;

protected:
	UPROPERTY(meta = (BindWidget))
	UUniformGridPanel * ItemCellsGrid;

	UPROPERTY(EditDefaultsOnly)
	int32 ItemsInRow = 5;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UTInventoryCellWidget> CellWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	TArray<UTInventoryCellWidget *> CellWidgets;

	UPROPERTY(meta = (BindWidgetOptional))
	UTInventoryCellWidget * PiastreCell;

};
