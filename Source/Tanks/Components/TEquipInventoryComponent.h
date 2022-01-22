// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TInventoryComponent.h"
#include "TEquipInventoryComponent.generated.h"



class UTInventoryWidget;

/**
 * 
 */
UCLASS()
class TANKS_API UTEquipInventoryComponent : public UTInventoryComponent
{
	GENERATED_BODY()
public:
	UTEquipInventoryComponent();
	
	virtual int32 GetMaxItemAmount(int32 SlotIndex, const FTInventoryItemInfo& Item) override;

protected:
	UPROPERTY(EditAnywhere)
	TMap<int32, EEquipSlot> EquipSlots;
	
};
