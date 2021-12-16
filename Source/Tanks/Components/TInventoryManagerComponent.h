// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TInventoryItemInfo.h"
#include "Components/ActorComponent.h"
#include "TInventoryManagerComponent.generated.h"

class UTInventoryComponent;
class UTInventoryWidget;
/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TANKS_API UTInventoryManagerComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UTInventoryManagerComponent();

	void Init(UTInventoryComponent * InInventoryComponent);

	FTInventoryItemInfo * GetItemData(FName ItemID) const;

protected:
	UPROPERTY()
	UTInventoryComponent * LocalInventoryComponent;

	UPROPERTY(EditAnywhere)
	UDataTable * InventoryItemsData;

	UPROPERTY()
	UTInventoryWidget * InventoryWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTInventoryWidget> InventoryWidgetClass;

	UPROPERTY(EditAnywhere)
	int32 MinInventorySize = 10;

};
