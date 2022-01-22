// Fill out your copyright notice in the Description page of Project Settings.


#include "TEquipInventoryComponent.h"


UTEquipInventoryComponent::UTEquipInventoryComponent()
{
	EquipSlots.Add(0, EEquipSlot::ES_Turret);
	EquipSlots.Add(1, EEquipSlot::ES_Track);
	EquipSlots.Add(2, EEquipSlot::ES_Trunk);
}

int32 UTEquipInventoryComponent::GetMaxItemAmount(int32 SlotIndex, const FTInventoryItemInfo& Item)
{
	if (Item.Type != EItemType::IT_Equipment || 
		!EquipSlots.Contains(SlotIndex) || 
		EquipSlots.FindChecked(SlotIndex) != Item.EquipSlot)
	{
		return 0;
	}

	return 1;
}
