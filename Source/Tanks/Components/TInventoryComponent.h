// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TInventoryItemInfo.h"
#include "Components/ActorComponent.h"
#include "TInventoryComponent.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API UTInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	void Init();
	
	const FTInventorySlotInfo * GetItem(int32 SlotIndex) const;
	virtual void SetItem(int32 SlotIndex, const FTInventorySlotInfo & Item );
	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FTInventoryItemInfo& Item);
	void ClearItem(int32 SlotIndex);
	const TMap<int32, FTInventorySlotInfo> &GetItems() const;
	int32 GetItemsNum() const;

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, FTInventorySlotInfo> Items;

	UPROPERTY(EditAnywhere)
	UDataTable * DefaultSetup;
	
};
