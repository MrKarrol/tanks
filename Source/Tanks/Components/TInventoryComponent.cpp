// Fill out your copyright notice in the Description page of Project Settings.

#include "TInventoryComponent.h"


void UTInventoryComponent::Init()
{
	if (DefaultSetup)
	{
		Items.Reset();

		TArray<FTInventorySlotInfo *> data;
		DefaultSetup->GetAllRows<FTInventorySlotInfo>(nullptr, data);

		int32 index = 1;
		for (const auto row : data)
			if (row)
				Items.Emplace(index++, *row);
	}
}

const FTInventorySlotInfo* UTInventoryComponent::GetItem(int32 SlotIndex) const
{
	return Items.Find(SlotIndex);
}

void UTInventoryComponent::SetItem(int32 SlotIndex, const FTInventorySlotInfo& Item)
{
	ClearItem(SlotIndex);
	Items.Add(SlotIndex, Item);
}

void UTInventoryComponent::ClearItem(int32 SlotIndex)
{
	Items.Remove(SlotIndex);
}

const TMap<int32, FTInventorySlotInfo>& UTInventoryComponent::GetItems() const
{
	return Items;
}

int32 UTInventoryComponent::GetItemsNum() const
{
	return Items.Num();
}
