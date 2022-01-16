// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryManagerComponent.h"

#include "TInventoryComponent.h"
#include "Tanks/Widgets/TInventoryCellWidget.h"
#include "Tanks/Widgets/TInventoryWidget.h"


UTInventoryManagerComponent::UTInventoryManagerComponent()
{
	
}

void UTInventoryManagerComponent::Init(UTInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;

	if (LocalInventoryComponent && InventoryItemsData && InventoryWidgetClass)
	{
		LocalInventoryComponent->Init();
		InventoryWidget = CreateWidget<UTInventoryWidget>(GetWorld(), InventoryWidgetClass);
		InventoryWidget->OnItemDrop.AddUObject(this, 
			&UTInventoryManagerComponent::OnItemDropped);
		InventoryWidget->AddToViewport();

		InventoryWidget->Init(FMath::Max(LocalInventoryComponent->GetItemsNum(), MinInventorySize));

		for (const auto & Item : LocalInventoryComponent->GetItems())
		{
			const FTInventoryItemInfo * ItemData = GetItemData(Item.Value.ID);
			if (ItemData)
			{
				InventoryWidget->AddItem(Item.Value, *ItemData, Item.Key);
			}
		}
	}
}

FTInventoryItemInfo* UTInventoryManagerComponent::GetItemData(FName ItemID) const
{
	if (InventoryItemsData)
		return InventoryItemsData->FindRow<FTInventoryItemInfo>(ItemID, "");
	return nullptr;
}

void UTInventoryManagerComponent::OnItemDropped(UTInventoryCellWidget * DraggedFrom, UTInventoryCellWidget * DroppedTo)
{
	const auto check_dragged_item = LocalInventoryComponent->GetItem(DraggedFrom->IndexInInventory);
	const auto check_dropped_to_item = LocalInventoryComponent->GetItem(DroppedTo->IndexInInventory);

	if (!DraggedFrom->HasItem())
		return;
	
	if (!DroppedTo->HasItem())
	{
		const auto dragged_item = *LocalInventoryComponent->GetItem(DraggedFrom->IndexInInventory);
		LocalInventoryComponent->SetItem(DroppedTo->IndexInInventory, dragged_item);
		LocalInventoryComponent->ClearItem(DraggedFrom->IndexInInventory);
		const FTInventoryItemInfo * ItemData = GetItemData(dragged_item.ID);
		DraggedFrom->Clear();
		DroppedTo->Clear();
		DroppedTo->AddItem(dragged_item, *ItemData);
	}
	else
	{
		const auto dragged_item = *LocalInventoryComponent->GetItem(DraggedFrom->IndexInInventory);
		const auto dropped_to_item = *LocalInventoryComponent->GetItem(DroppedTo->IndexInInventory);
		LocalInventoryComponent->SetItem(DroppedTo->IndexInInventory, dragged_item);
		LocalInventoryComponent->SetItem(DraggedFrom->IndexInInventory, dropped_to_item);
		const FTInventoryItemInfo * DraggedItemData = GetItemData(dragged_item.ID);
		const FTInventoryItemInfo * DroppedToItemData = GetItemData(dropped_to_item.ID);
		DraggedFrom->Clear();
		DroppedTo->Clear();
		DraggedFrom->AddItem(dropped_to_item, *DroppedToItemData);
		DroppedTo->AddItem(dragged_item, *DraggedItemData);
	}
}
