// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/DragDropOperation.h"
#include "TInventoryDragDropOperation.generated.h"

class UTInventoryCellWidget;

/**
 * 
 */
UCLASS()
class TANKS_API UTInventoryDragDropOperation : public UDragDropOperation
{
	GENERATED_BODY()
	
public:  
	UPROPERTY()
	UTInventoryCellWidget* SourceCell;
	
};
