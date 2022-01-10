// Fill out your copyright notice in the Description page of Project Settings.


#include "TPlayerController.h"

#include "TPlayerPawn.h"
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

	InputComponent->BindAxis("MoveForward", this, &ATPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATPlayerController::MoveRight);

	InputComponent->BindAxis("MoveTurretUp", this, &ATPlayerController::MoveTurretUp);
	InputComponent->BindAxis("MoveTurretRight", this, &ATPlayerController::MoveTurretRight);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPlayerController::StartFire);
	// possible bug here. if key for Fire action will change, trigger for spawning turret helper also will change
	// need to find way to broadcast when left mouse button is released
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ATPlayerController::OnLeftMouseButtonUp);
	InputComponent->BindAction("AlternateFire", EInputEvent::IE_Pressed, this, &ATPlayerController::AlternateFire);
	InputComponent->BindAction("SwapGuns", EInputEvent::IE_Pressed, this, &ATPlayerController::SwapGuns);
}

void ATPlayerController::OnLeftMouseButtonUp()
{
	StopFire();
	
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

void ATPlayerController::StartFire()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->StartFire();
}

void ATPlayerController::StopFire()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->StopFire();
}

void ATPlayerController::AlternateFire()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->AlternateFire();
}

void ATPlayerController::SwapGuns()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->SwapGuns();
}

void ATPlayerController::MoveForward(float Axis)
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->MoveForward(Axis);
}

void ATPlayerController::MoveRight(float Axis)
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->MoveRight(Axis);
}

void ATPlayerController::MoveTurretUp(float Axis)
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->MoveTurretUp(Axis);
}

void ATPlayerController::MoveTurretRight(float Axis)
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->MoveTurretRight(Axis);
}

void ATPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameAndUI());
}
