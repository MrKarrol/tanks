// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tanks/Components/TInventoryItemInfo.h"
#include "TInventoryCellWidget.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class TANKS_API UTInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	bool HasItem() const noexcept;
	bool AddItem(const FTInventorySlotInfo & Item, const FTInventoryItemInfo & ItemInfo);
	void Clear();

	const FTInventorySlotInfo & GetItem() const;

	int32 IndexInInventory = -1;

protected:
	bool bHasItem = false;

	UPROPERTY(meta = (BindWidget))
	UImage * ItemImage;

	UPROPERTY(meta = (BindWidget))
	UTextBlock * CountText;

	UPROPERTY()
	FTInventorySlotInfo StoredItem;
	
};
