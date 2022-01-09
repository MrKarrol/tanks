// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerController.h"

#include "Tanks/Widgets/THUD.h"


ATPlayerController::ATPlayerController()
{
	bEnableClickEvents = true;
}

void ATPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ShowMiniMap", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowMinimap);
	InputComponent->BindAction("ShowTurretHelpers", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowTurretHelpers);
	InputComponent->BindAction("ShowInventory", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowInventory);
	InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowPauseMenu);
}

void ATPlayerController::OnLeftMouseButtonUp()
{
	if (OnMouseButtonUp.IsBound())
		OnMouseButtonUp.Broadcast();
}

void ATPlayerController::ShowMinimap()
{
	static constexpr auto minimap_type = ESideWidgetType::SWT_MiniMap;
	if (const auto hud = Cast<ATHUD>(GetHUD()))
	{
		if (hud->IsSideWidgetShown(minimap_type))
			hud->HideSideWidget(minimap_type);
		else
			hud->ShowSideWidget(minimap_type);
	}
}

void ATPlayerController::ShowTurretHelpers()
{
	static constexpr auto turret_helpers_type = ESideWidgetType::SWT_TurretHelpers;
	if (const auto hud = Cast<ATHUD>(GetHUD()))
	{
		if (hud->IsSideWidgetShown(turret_helpers_type))
			hud->HideSideWidget(turret_helpers_type);
		else
			hud->ShowSideWidget(turret_helpers_type);
	}
}

void ATPlayerController::ShowInventory()
{
	static constexpr auto inventory_type = ESideWidgetType::SWT_Inventory;
	if (const auto hud = Cast<ATHUD>(GetHUD()))
	{
		if (hud->IsSideWidgetShown(inventory_type))
			hud->HideSideWidget(inventory_type);
		else
			hud->ShowSideWidget(inventory_type);
	}
}

void ATPlayerController::ShowPauseMenu()
{
	static constexpr auto pause_menu_type = EMainWidgetType::MWT_PauseMenu;
	if (const auto hud = Cast<ATHUD>(GetHUD()))
	{
		if (hud->IsMainWidgetShown(pause_menu_type))
		{
			hud->HideMainWidget();
		}
		else
		{
			hud->ShowMainWidget(pause_menu_type);
		}
	}
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}
