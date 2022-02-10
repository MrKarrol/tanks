// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryManagerComponent.h"

#include "TInventoryComponent.h"
#include "Tanks/Widgets/TInventoryCellWidget.h"
#include "Tanks/Widgets/TInventoryWidget.h"


UTInventoryManagerComponent::UTInventoryManagerComponent()
{
	
}

void UTInventoryManagerComponent::Init(UTInventoryComponent* InventoryComponent)
{
	Inventory = InventoryComponent;
	Inventory->Init();
}

FTInventoryItemInfo* UTInventoryManagerComponent::GetItemData(FName ItemID) const
{
	if (InventoryItemsData)
		return InventoryItemsData->FindRow<FTInventoryItemInfo>(ItemID, "");
	return nullptr;
}

void UTInventoryManagerComponent::InitEquipment(UTInventoryComponent* EquipmentInventoryComponent)
{
	EquipInventory = EquipmentInventoryComponent;
}

void UTInventoryManagerComponent::ShowInventory()
{
	if (InventoryWidget)
	{
		InventoryWidget->RemoveFromViewport();
		InventoryWidget = nullptr;
		return;
	}
	if (Inventory && InventoryItemsData && InventoryWidgetClass)
	{
		InventoryWidget = CreateWidget<UTInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->OnItemDrop.AddUObject(this, 
			&UTInventoryManagerComponent::OnItemDropped);
		InventoryWidget->RepresentedInventory = Inventory;
		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(Inventory->GetItemsNum(), MinInventorySize));

		for (const auto & Item : Inventory->GetItems())
		{
			const FTInventoryItemInfo * ItemData = GetItemData(Item.Value.ID);
			if (ItemData)
			{
				InventoryWidget->AddItem(Item.Value, *ItemData, Item.Key);
			}
		}
	}
}

void UTInventoryManagerComponent::ShowEquipmentInventory()
{
	if (EquipInventoryWidget)
	{
		EquipInventoryWidget->RemoveFromViewport();
		EquipInventoryWidget = nullptr;
		return;
	}
	
	if (EquipInventoryWidgetClass)
	{
		EquipInventoryWidget = CreateWidget<UTInventoryWidget>(GetWorld(), 
		EquipInventoryWidgetClass);
		EquipInventoryWidget->OnItemDrop.AddUObject(this, 
			&UTInventoryManagerComponent::OnItemDropped);
		EquipInventoryWidget->RepresentedInventory = EquipInventory;
		EquipInventoryWidget->AddToViewport();
	}
}

void UTInventoryManagerComponent::OnItemDropped(UTInventoryCellWidget * DraggedFrom, UTInventoryCellWidget * DroppedTo)
{
	const auto FromInventory = DraggedFrom->ParentInventoryWidget->RepresentedInventory;
	const auto ToInventory = DroppedTo->ParentInventoryWidget->RepresentedInventory;
	if (!FromInventory || !ToInventory || !DraggedFrom->HasItem())
	{
		return;
	}

	if (!DroppedTo->HasItem())
	{
		auto dragged_item = *FromInventory->GetItem(DraggedFrom->IndexInInventory);
		const auto dragged_item_data = GetItemData(dragged_item.ID);
		
		auto item_to_drop = dragged_item;
		item_to_drop.Amount = ToInventory->GetMaxItemAmount(DroppedTo->IndexInInventory, *dragged_item_data);
		if (item_to_drop.Amount == 0)	
			return;
		if (item_to_drop.Amount == -1)
			item_to_drop.Amount = dragged_item.Amount;
		dragged_item.Amount -= item_to_drop.Amount;
		
		DraggedFrom->Clear();
		if (dragged_item.Amount)
		{
			DraggedFrom->AddItem(dragged_item, *dragged_item_data);
			FromInventory->SetItem(DraggedFrom->IndexInInventory, dragged_item);
		}
		else
			FromInventory->ClearItem(DraggedFrom->IndexInInventory);
		DroppedTo->Clear();
		DroppedTo->AddItem(item_to_drop, *dragged_item_data);
		ToInventory->SetItem(DroppedTo->IndexInInventory, item_to_drop);
	}
	else
	{
		// make swap for different elements in different or same inventories
		// make elements to stack
	}
}
