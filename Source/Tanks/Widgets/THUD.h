// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "THUD.generated.h"

UENUM()
enum class EMainWidgetType : uint8
{
	MWT_NoWidget = 0,
	MWT_MainMenu,
	MWT_PauseMenu,
	MWT_GameOverMenu,
};

UENUM()
enum class ESideWidgetType : uint8
{
	SWT_NoWidget = 0,
	SWT_PlayerState,
	SWT_MiniMap,
	SWT_TurretHelpers,
	SWT_Inventory,
	SWT_EquipmentInventory,
};

class UUserWidget;
class UTWidget;

UCLASS()
class TANKS_API ATHUD : public AHUD
{
	GENERATED_BODY()

public:
	ATHUD();
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ShowMainMenu();
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void ShowPlayerState();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	UUserWidget * ShowMainWidget(const EMainWidgetType main_widget_type, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	UUserWidget * ShowSideWidget(const ESideWidgetType side_widget_type, const int32 ZOrder = 0);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void HideMainWidget();

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void HideSideWidget(const ESideWidgetType side_widget_type);

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	bool IsMainWidgetShown(const EMainWidgetType type) const;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	bool IsSideWidgetShown(const ESideWidgetType type) const;

	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	TArray<ESideWidgetType> ShowedSideWidgetsTypes() const;
	
	UFUNCTION(BlueprintCallable, Category = "UMG Game")
	void HideAllSideWidgets();

	virtual void Tick(float DeltaTime) override;

private:
	void SetupPlayerState();
	void SetupMiniMap();
	void ManagePauseMenuCreation(UTWidget *);
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TMap<EMainWidgetType, TSubclassOf<UUserWidget>> MainWidgetClasses;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UMG Game")
	TMap<ESideWidgetType, TSubclassOf<UUserWidget>> SideWidgetClasses;

private:
	EMainWidgetType showed_main_widget_type = EMainWidgetType::MWT_NoWidget;
	UUserWidget* showed_main_widget;

	TMap<ESideWidgetType, UUserWidget *> showed_side_widgets;
	
};
