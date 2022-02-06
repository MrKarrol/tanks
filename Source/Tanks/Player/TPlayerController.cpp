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
	InputComponent->BindAction("ShowInventory", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowInventories);
	InputComponent->BindAction("Pause", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowPauseMenu);

	InputComponent->BindAxis("MoveForward", this, &ATPlayerController::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ATPlayerController::MoveRight);

	InputComponent->BindAxis("MoveTurretUp", this, &ATPlayerController::MoveTurretUp);
	InputComponent->BindAxis("MoveTurretRight", this, &ATPlayerController::MoveTurretRight);

	InputComponent->BindAction("Fire", EInputEvent::IE_Pressed, this, &ATPlayerController::TankStartFire);
	// possible bug here. if key for Fire action will change, trigger for spawning turret helper also will change
	// need to find way to broadcast when left mouse button is released
	InputComponent->BindAction("Fire", EInputEvent::IE_Released, this, &ATPlayerController::OnLeftMouseButtonUp);
	InputComponent->BindAction("AlternateFire", EInputEvent::IE_Pressed, this, &ATPlayerController::TankAlternateFire);
	InputComponent->BindAction("SwapGuns", EInputEvent::IE_Pressed, this, &ATPlayerController::TankSwapGuns);
	InputComponent->BindAction("Browse", EInputEvent::IE_Pressed, this, &ATPlayerController::ShowBrowseWidget);
}

void ATPlayerController::OnLeftMouseButtonUp()
{
	TankStopFire();
	
	if (OnMouseButtonUp.IsBound())
		OnMouseButtonUp.Broadcast();
}

void ATPlayerController::ShowMinimap()
{
	if (const auto hud = Cast<ATHUD>(GetHUD()))
		hud->ShowSideWidget(ESideWidgetType::SWT_MiniMap);
}

void ATPlayerController::ShowTurretHelpers()
{
	if (const auto hud = Cast<ATHUD>(GetHUD()))
		hud->ShowSideWidget(ESideWidgetType::SWT_TurretHelpers);
}

void ATPlayerController::ShowInventories()
{
	ShowInventory();
	ShowEquipmentInventory();
}

void ATPlayerController::ShowInventory()
{
	if (const auto hud = Cast<ATHUD>(GetHUD()))
		hud->ShowSideWidget(ESideWidgetType::SWT_Inventory);
}

void ATPlayerController::ShowEquipmentInventory()
{
	if (const auto hud = Cast<ATHUD>(GetHUD()))
		hud->ShowSideWidget(ESideWidgetType::SWT_EquipmentInventory);
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

void ATPlayerController::ShowBrowseWidget()
{
	if (const auto hud = Cast<ATHUD>(GetHUD()))
		hud->ShowSideWidget(ESideWidgetType::SWT_BrowseWidget);
}

void ATPlayerController::TankStartFire()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->StartFire();
}

void ATPlayerController::TankStopFire()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->StopFire();
}

void ATPlayerController::TankAlternateFire()
{
	if (const auto pawn = Cast<ATPlayerPawn>(GetPawn()))
		pawn->AlternateFire();
}

void ATPlayerController::TankSwapGuns()
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
