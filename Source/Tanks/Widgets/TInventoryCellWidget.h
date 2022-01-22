// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tanks/Components/TInventoryItemInfo.h"
#include "TInventoryCellWidget.generated.h"

class UTextBlock;
class UImage;
class UTInventoryWidget;

/**
 * 
 */
UCLASS()
class TANKS_API UTInventoryCellWidget : public UUserWidget
{
	GENERATED_BODY()

	DECLARE_MULTICAST_DELEGATE_TwoParams(FOnItemDrop, 
	UTInventoryCellWidget * /*DraggedFrom*/, UTInventoryCellWidget * /*DroppedTo*/);
	
public:
	bool HasItem() const noexcept;
	bool AddItem(const FTInventorySlotInfo & Item, const FTInventoryItemInfo & ItemInfo);
	void Clear();

	const FTInventorySlotInfo & GetItem() const;

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, 
	                                       const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, 
	                                  const FPointerEvent& InMouseEvent, 
	                                  UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, 
	                          const FDragDropEvent& InDragDropEvent, 
	                          UDragDropOperation* InOperation) override;

public:
	UPROPERTY(EditAnywhere)
	int32 IndexInInventory = -1;
	
	FOnItemDrop OnItemDrop;

	UPROPERTY()
	UTInventoryWidget * ParentInventoryWidget;

protected:
	bool bHasItem = false;

	UPROPERTY(meta = (BindWidget))
	UImage * ItemImage;

	UPROPERTY(EditAnywhere, Category = "Icons")
	TSoftObjectPtr<UTexture2D> NoItemBrush;

	UPROPERTY(meta = (BindWidget))
	UTextBlock * CountText;

	UPROPERTY()
	FTInventorySlotInfo StoredItem;

	UPROPERTY(EditDefaultsOnly)
	bool bIsDraggable = true;
	
};
