// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryManagerComponent.h"

#include "TInventoryComponent.h"
#include "Tanks/Widgets/TInventoryWidget.h"


UTInventoryManagerComponent::UTInventoryManagerComponent()
{
	
}

void UTInventoryManagerComponent::Init(UTInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;

	if (LocalInventoryComponent && InventoryItemsData)
	{
		ensure(InventoryWidgetClass);
		InventoryWidget = CreateWidget<UTInventoryWidget>(GetWorld(), InventoryWidgetClass);
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
