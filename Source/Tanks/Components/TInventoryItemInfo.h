#pragma once
#include "Engine/DataTable.h"

#include "TInventoryItemInfo.generated.h"

UENUM()
enum class EItemType : uint8
{
	IT_Miscellaneous,
	IT_Currency,
	IT_Equipment,
	IT_Consumable,
};

UENUM()
enum class EItemRarity : uint8
{
	IR_Common,
	IR_Uncommon,
	IR_Rare,
	IR_Epic,
	IR_Legendary,
};

UENUM()
enum class EEquipSlot : uint8
{
	ES_NoSlot = 0,
	ES_Turret,
	ES_Track,
	ES_Trunk,
};

USTRUCT(BlueprintType)
struct FTInventoryItemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// info
	UPROPERTY(EditAnywhere, Category="General")
	FName ID;

	UPROPERTY(EditAnywhere, Category="General")
	FText Name;

	UPROPERTY(EditAnywhere, Category="General")
	FText Description;

	// type and rarity
	UPROPERTY(EditAnywhere, Category="Typing")
	EItemType Type;

	UPROPERTY(EditAnywhere, Category="Typing")
	EItemRarity Rarity;

	UPROPERTY(EditAnywhere, Category="Typing")
	EEquipSlot EquipSlot;

	// visual representation
	UPROPERTY(EditAnywhere, Category="Visual")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, Category="Visual")
	TSoftObjectPtr<UStaticMesh> Mesh;

	// stats
	UPROPERTY(EditAnywhere, Category="Stats")
	int32 Damage;

	UPROPERTY(EditAnywhere, Category="Stats")
	int32 Armor;

	UPROPERTY(EditAnywhere, Category="Stats")
	int32 Intelligence;
	
};

USTRUCT(BlueprintType)
struct FTInventorySlotInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, Category="General")
	FName ID;

	UPROPERTY(EditAnywhere, Category="General")
	int32 Amount;

};