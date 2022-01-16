// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryWidget.h"

#include "TInventoryCellWidget.h"
#include "Tanks/Components/TInventoryItemInfo.h"
#include "Components/UniformGridPanel.h"


void UTInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTInventoryWidget::Init(int32 ItemsNum)
{
	if (ItemCellsGrid)
	{
		ItemCellsGrid->ClearChildren();
		for (int32 i = 0; i < ItemsNum; ++i)
		{
			const auto widget = CreateCellWidget();
			widget->IndexInInventory = i;
			ItemCellsGrid->AddChildToUniformGrid(widget, i / ItemsInRow, i % ItemsInRow);
		}
	}
}

bool UTInventoryWidget::AddItem(const FTInventorySlotInfo& Item, const FTInventoryItemInfo& ItemInfo, int32 SlotPosition)
{
	if (ItemInfo.Type == EItemType::IT_Currency)
	{
		if (PiastreCell)
		{
			return PiastreCell->AddItem(Item, ItemInfo);
		}
		return false;
	}

	if (ItemCellsGrid)
	{
		UTInventoryCellWidget * WidgetToAddItem = nullptr;
		auto WidgetToAddItemPtr =
			CellWidgets.FindByPredicate([SlotPosition](UTInventoryCellWidget * widget)
			{
				return widget && widget->IndexInInventory == SlotPosition;
			});

		if (WidgetToAddItemPtr)
		{
			WidgetToAddItem = *WidgetToAddItemPtr;
		}
		else
		{
			for (const auto & CellWidget : CellWidgets)
			{
				if (!CellWidget->HasItem())
				{
					WidgetToAddItem = CellWidget;
					break;
				}
			}
		}

		if (WidgetToAddItem)
		{
			return WidgetToAddItem->AddItem(Item, ItemInfo);
		}
	}
	
	return true;
}

UTInventoryCellWidget* UTInventoryWidget::CreateCellWidget()
{
	if (CellWidgetClass)
	{
		const auto cell_widget = CreateWidget<UTInventoryCellWidget>(this, CellWidgetClass);
		cell_widget->OnItemDrop.AddUObject(this, &UTInventoryWidget::OnItemDropped);
		CellWidgets.Add(cell_widget);
		return cell_widget;
	}
	return nullptr;
}

void UTInventoryWidget::OnItemDropped(UTInventoryCellWidget* DraggedFrom, UTInventoryCellWidget* DroppedTo)
{
	if (OnItemDrop.IsBound())
	{
		OnItemDrop.Broadcast(DraggedFrom, DroppedTo);
	}
}
