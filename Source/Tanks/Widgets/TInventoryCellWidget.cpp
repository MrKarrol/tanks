// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryCellWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"


bool UTInventoryCellWidget::HasItem() const noexcept
{
	return bHasItem;
}

bool UTInventoryCellWidget::AddItem(const FTInventorySlotInfo& Item, const FTInventoryItemInfo& ItemInfo)
{
	if (bHasItem)
		return false;

	if (ItemImage)
	{
		ItemImage->SetBrushFromTexture(ItemInfo.Icon.Get());
		ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 1.f));
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(Item.Amount)));
	}

	return true;
}

void UTInventoryCellWidget::Clear()
{
	if (ItemImage)
	{
		ItemImage->SetBrush(FSlateBrush());
		ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f, 0.f));
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString("0"));
	}

	bHasItem = false;
	StoredItem = FTInventorySlotInfo();
}

const FTInventorySlotInfo& UTInventoryCellWidget::GetItem() const
{
	return StoredItem;
}
