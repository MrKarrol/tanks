// Fill out your copyright notice in the Description page of Project Settings.


#include "TInventoryCellWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "TInventoryDragDropOperation.h"


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
		ItemImage->SetColorAndOpacity(FLinearColor(1.f, 1.f, 1.f));
	}

	if (CountText)
	{
		CountText->SetText(FText::FromString(FString::FromInt(Item.Amount)));
	}

	bHasItem = true;
	
	return true;
}

void UTInventoryCellWidget::Clear()
{
	if (NoItemBrush)
	{
		ItemImage->SetBrushFromTexture(NoItemBrush.Get());
	}
	else
	{
		ItemImage->SetBrush({});
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

FReply UTInventoryCellWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (bIsDraggable && bHasItem &&     
		InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		return UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, 
			EKeys::LeftMouseButton).NativeReply;
	}
	return FReply::Handled();
}

void UTInventoryCellWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent,
	UDragDropOperation*& OutOperation)
{
	OutOperation = UWidgetBlueprintLibrary::CreateDragDropOperation(
		UTInventoryDragDropOperation::StaticClass());
	if (OutOperation) 
	{
		UTInventoryDragDropOperation* InventoryDragDropOperation = 
			Cast<UTInventoryDragDropOperation>(OutOperation);

		InventoryDragDropOperation->SourceCell = this;
		InventoryDragDropOperation->DefaultDragVisual = this;
	}
	else 
	{
		Super::NativeOnDragDetected(InGeometry, InMouseEvent, OutOperation);
	}
}

bool UTInventoryCellWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent,
	UDragDropOperation* InOperation)
{
	UTInventoryDragDropOperation* InventoryDragDropOperation = 
		Cast<UTInventoryDragDropOperation>(InOperation);
	if (InventoryDragDropOperation && InventoryDragDropOperation->SourceCell != this)
	{
		if (OnItemDrop.IsBound())
		{
			OnItemDrop.Broadcast(InventoryDragDropOperation->SourceCell, this);
		}
	}

	return Super::NativeOnDrop(InGeometry, InDragDropEvent, InOperation);
}
