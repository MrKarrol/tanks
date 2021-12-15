// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryManagerComponent.h"

#include "TInventoryComponent.h"
#include "Components/SplineComponent.h"


UTInventoryManagerComponent::UTInventoryManagerComponent()
{
	
}

void UTInventoryManagerComponent::Init(UTInventoryComponent* InInventoryComponent)
{
	LocalInventoryComponent = InInventoryComponent;

	if (LocalInventoryComponent && InventoryItemsData)
	{
		for (const auto & Item : LocalInventoryComponent->GetItems())
		{
			const FTInventoryItemInfo * ItemData = GetItemData(Item.Value.ID);
			if (ItemData)
			{
				// TODO manage item
				const FString ItemDataStr = ItemData->Name.ToString() + ": " +
					FString::FromInt(Item.Value.Amount);
				GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Emerald,
					ItemDataStr);
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
