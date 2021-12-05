#include "THUD.h"

#include "Tanks/Widgets/TWidget.h"
#include "Tanks/Widgets/PlayerStateWidget.h"
#include "Tanks/Player/TPlayerPawn.h"
#include "Tanks/Components/THealthComponent.h"
#include "Tanks/Core/GameModes/TGameMode.h"
#include "Tanks/Guns/TGun.h"


ATHUD::ATHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATHUD::ShowMainMenu()
{
	if (MainWidgetClasses.Contains(EMainWidgetType::MWT_MainMenu))
		ShowMainWidget(EMainWidgetType::MWT_MainMenu);
}

void ATHUD::ShowPlayerState()
{
	// if (auto player_state_widget = WidgetClasses.Find(EWidgetType::WT_PlayerState))
}

void ATHUD::BeginPlay()
{
	Super::BeginPlay();
}

void ATHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATHUD::SetupPlayerState()
{
	const auto player_state_widget = Cast<UPlayerStateWidget>(*showed_side_widgets.Find(ESideWidgetType::SWT_PlayerState));
	const auto player = Cast<ATPlayerPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (player_state_widget && player)
	{
		player_state_widget->SetMaxHealth(player->GetHealthComponent()->GetMaxHealth());
		player->GetHealthComponent()->OnHealthChangedDelegate.AddUObject(player_state_widget, &UPlayerStateWidget::SetHealth);
		player->OnChangeScoreDelegate.BindUObject(player_state_widget, &UPlayerStateWidget::SetScore);
		if (const auto game_mode = Cast<ATGameMode>(GetWorld()->GetAuthGameMode()))
		{
			game_mode->OnEnemiesNumberChangedDelegate.AddUObject(player_state_widget, &UPlayerStateWidget::SetEnemiesCount);
		}
		auto BindGun = [player_state_widget](const EGunSlot gun_slot, ATGun * gun)
		{
			switch(gun_slot)
			{
			case EGunSlot::GS_First:
				player_state_widget->SetFirstGunName(gun->GunName);
				player_state_widget->SetFirstGunAmmo(gun->GetCurrentAmmo());
				gun->OnChangeAmmoCapacityDelegate.BindUObject(player_state_widget, &UPlayerStateWidget::SetFirstGunAmmo);
				break;
			case EGunSlot::GS_Second:
				player_state_widget->SetSecondGunName(gun->GunName);
				player_state_widget->SetSecondGunAmmo(gun->GetCurrentAmmo());
				gun->OnChangeAmmoCapacityDelegate.BindUObject(player_state_widget, &UPlayerStateWidget::SetSecondGunAmmo);
				break;
			default: break;
			}
		};
		const auto guns = player->GetGuns();
		for (const auto & gun : guns)
			BindGun(gun.Key, gun.Value);

		player->OnGunChangeDelegate.BindLambda(BindGun);
	}
}

void ATHUD::ShowMainWidget(const EMainWidgetType main_widget_type, const int32 ZOrder)
{
	HideMainWidget();
	if (! MainWidgetClasses.Contains(main_widget_type))
		return;
	if (const auto new_main_class = MainWidgetClasses[main_widget_type])
	{
		showed_main_widget = CreateWidget<UUserWidget>(GetWorld(), new_main_class);
		if (showed_main_widget)
		{
			showed_main_widget->AddToViewport(ZOrder);
			if (const auto twidget = Cast<UTWidget>(showed_main_widget))
				twidget->OnNeedToRemove.BindUObject(this, &ATHUD::HideMainWidget);
		}
	}
}

void ATHUD::ShowSideWidget(const ESideWidgetType side_widget_type, const int32 ZOrder)
{
	HideSideWidget(side_widget_type);
	if (! SideWidgetClasses.Contains(side_widget_type))
		return;
	
	if (const auto new_side_class = SideWidgetClasses[side_widget_type])
	{
		showed_side_widgets.Add(side_widget_type, CreateWidget<UUserWidget>(GetWorld(), new_side_class));
		if (const auto widget = *showed_side_widgets.Find(side_widget_type))
			widget->AddToViewport(ZOrder);
		
		if (side_widget_type == ESideWidgetType::SWT_PlayerState)
			SetupPlayerState();
	}
}

void ATHUD::HideMainWidget()
{
	if (showed_main_widget)
	{
		showed_main_widget->RemoveFromViewport();
		showed_main_widget = nullptr;
	}
}

void ATHUD::HideSideWidget(const ESideWidgetType side_widget_type)
{
	if (showed_side_widgets.Contains(side_widget_type))
	{
		if (showed_side_widgets[side_widget_type])
		{
			if (const auto game_mode = Cast<ATGameMode>(GetWorld()->GetAuthGameMode()))
			{
				game_mode->OnEnemiesNumberChangedDelegate.RemoveAll(showed_side_widgets[side_widget_type]);
			}
			showed_side_widgets[side_widget_type]->RemoveFromViewport();
		}
		showed_side_widgets.Remove(side_widget_type);
	}
}
