// Fill out your copyright notice in the Description page of Project Settings.

#include "TInventoryComponent.h"


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
