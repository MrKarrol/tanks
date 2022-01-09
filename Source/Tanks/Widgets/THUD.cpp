#include "THUD.h"

#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Tanks/Widgets/TWidget.h"
#include "Tanks/Widgets/PlayerStateWidget.h"
#include "Tanks/Widgets/TMiniMapHolder.h"
#include "Tanks/Player/TPlayerPawn.h"
#include "Tanks/Components/THealthComponent.h"
#include "Tanks/Core/GameModes/TGameMode.h"
#include "Tanks/Guns/TGun.h"
#include "Tanks/Meta/TWall.h"
#include "Tanks/Meta/TFloor.h"


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

	if (showed_side_widgets.Contains(ESideWidgetType::SWT_MiniMap))
		SetupMiniMap();
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

void ATHUD::SetupMiniMap()
{
	if (!showed_side_widgets.Contains(ESideWidgetType::SWT_MiniMap))
		return;
	const auto mini_map_widget = Cast<UTMiniMapHolder>(showed_side_widgets[ESideWidgetType::SWT_MiniMap]);
	if (!mini_map_widget)
		return;
	
	TArray<AActor*> walls;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATWall::StaticClass(), walls);
	if (!walls.Num())
		return;

	// int32 Xfinal, Yfinal;
	TArray<TArray<FVector2D>> bounds_to_paint;

	auto GetXYExtremePoints = [&bounds_to_paint](const AActor * actor) -> TArray<FVector2D>
	{
		FBox actual_size;
		const FVector actor_location = actor->GetActorLocation();
		for (const auto component : actor->GetComponents())
		{
			if (const auto static_mesh = Cast<UStaticMeshComponent>(component))
			{
				FBox size_box;
				static_mesh->GetLocalBounds(size_box.Min, size_box.Max);
				const auto transform = static_mesh->GetRelativeTransform();
				// const auto mesh_location = actor_location + transform.GetLocation();
				const auto mesh_location = actor_location;
				const auto scaled_size_box = FBox(size_box.Min * transform.GetScale3D(), size_box.Max * transform.GetScale3D());
				
				actual_size = FBox(mesh_location + scaled_size_box.Min,  mesh_location + scaled_size_box.Max);
				break;
			}
			else if (const auto skeletal_mesh = Cast<USkeletalMeshComponent>(component))
			{
				const auto size_box = skeletal_mesh->GetCachedLocalBounds().GetBox();
				const auto transform = skeletal_mesh->GetRelativeTransform();
				// const auto mesh_location = actor_location + transform.GetLocation();
				const auto mesh_location = actor_location;
				const auto scaled_size_box = FBox(size_box.Min * transform.GetScale3D(), size_box.Max * transform.GetScale3D());
				
				actual_size = FBox(mesh_location + scaled_size_box.Min,  mesh_location + scaled_size_box.Max);
				break;
			}
		}
		if (actual_size.Min.IsNearlyZero() || actual_size.Max.IsNearlyZero())
			return {};

		FTransform const Transform(actor->GetActorRotation().Quaternion());
		const auto first_point = actual_size.GetCenter() + Transform.TransformPosition(FVector(actual_size.GetExtent().X, actual_size.GetExtent().Y, actual_size.GetExtent().Z));
		const auto second_point = actual_size.GetCenter() + Transform.TransformPosition(FVector(actual_size.GetExtent().X, -actual_size.GetExtent().Y, actual_size.GetExtent().Z));
		const auto third_point = actual_size.GetCenter() + Transform.TransformPosition(FVector(-actual_size.GetExtent().X, -actual_size.GetExtent().Y, actual_size.GetExtent().Z));
		const auto forth_point = actual_size.GetCenter() + Transform.TransformPosition(FVector(-actual_size.GetExtent().X, actual_size.GetExtent().Y, actual_size.GetExtent().Z));
		auto Get2D = [](const FVector &vec)
		{
			return FVector2D(vec.X, vec.Y);
		};
		TArray<FVector2D> points;
		points.Add(Get2D(first_point));
		points.Add(Get2D(second_point));
		points.Add(Get2D(third_point));
		points.Add(Get2D(forth_point));
		points.Add(Get2D(first_point));

		return points;
	};
	
	for (const auto wall : walls)
	{
		auto points = GetXYExtremePoints(wall);
		if (points.Num())
			bounds_to_paint.Add(std::move(points));
	}

	if (! bounds_to_paint.Num())
		if (! bounds_to_paint[0].Num())
			return;
	float lowestY = bounds_to_paint[0][0].Y;
	float highestY = bounds_to_paint[0][0].Y;
	float lowestX = bounds_to_paint[0][0].X;
	float highestX = bounds_to_paint[0][0].X;
	for (const auto &points_array : bounds_to_paint)
		for (const auto &point : points_array)
		{
			if (point.X > highestX)
				highestX = point.X;
			if (point.X < lowestX)
				lowestX = point.X;
			if (point.Y > highestY)
				highestY = point.Y;
			if (point.Y < lowestY)
				lowestY = point.Y;
		}
	const float finalX = highestX - lowestX;
	const float finalY = highestY - lowestY;

	const float sizeX = mini_map_widget->GetMiniMapSize() * finalX / FMath::Max(finalX, finalY);
	const float sizeY = mini_map_widget->GetMiniMapSize() * finalY / FMath::Max(finalX, finalY);

	auto ConvertPoint = [lowestX, lowestY, sizeX, sizeY, finalX, finalY](FVector2D &point)
	{
		point.X -= lowestX;
		point.X = point.X * sizeX / finalX;
		point.Y -= lowestY;
		point.Y = sizeY - point.Y * sizeY / finalY;
	};

	TArray<AActor*> all_actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), all_actors);

	TArray<FVector2D> player_points;
	for (const auto actor : all_actors)
	{
		if (Cast<ATWall>(actor) || Cast<ATFloor>(actor) || Cast<ATGun>(actor))
			continue;

		auto points = GetXYExtremePoints(actor);
		if (!points.Num())
			continue;

		if (Cast<ATPlayerPawn>(actor))
			player_points = std::move(points);
		else
			bounds_to_paint.Add(std::move(points));
	}

	// for (const auto &point_array : bounds_to_paint)
	// {
	// 	DrawDebugLine(GetWorld(), FVector(point_array[0].X, point_array[0].Y, 200) , FVector(point_array[1].X, point_array[1].Y, 200), FColor::Red, false, 0.f, 5, 7.f);
	// 	DrawDebugLine(GetWorld(), FVector(point_array[1].X, point_array[1].Y, 200) , FVector(point_array[2].X, point_array[2].Y, 200), FColor::Red, false, 0.f, 5, 7.f);
	// 	DrawDebugLine(GetWorld(), FVector(point_array[2].X, point_array[2].Y, 200) , FVector(point_array[3].X, point_array[3].Y, 200), FColor::Red, false, 0.f, 5, 7.f);
	// 	DrawDebugLine(GetWorld(), FVector(point_array[3].X, point_array[3].Y, 200) , FVector(point_array[4].X, point_array[4].Y, 200), FColor::Red, false, 0.f, 5, 7.f);
	// }

	for (auto &point_array : bounds_to_paint)
		for (auto &point : point_array)
		{
			ConvertPoint(point);
		}

	for (auto &point : player_points)
		ConvertPoint(point);
	
	mini_map_widget->SetBoundsToPaint(sizeX, sizeY, std::move(bounds_to_paint), std::move(player_points));
}

UUserWidget * ATHUD::ShowMainWidget(const EMainWidgetType main_widget_type, const int32 ZOrder)
{
	HideMainWidget();
	if (! MainWidgetClasses.Contains(main_widget_type))
		return nullptr;
	if (const auto new_main_class = MainWidgetClasses[main_widget_type])
	{
		showed_main_widget = CreateWidget<UUserWidget>(GetWorld(), new_main_class);
		if (showed_main_widget)
		{
			showed_main_widget->AddToViewport(ZOrder);
			if (const auto twidget = Cast<UTWidget>(showed_main_widget))
			{
				twidget->OnNeedToRemove.AddUObject(this, &ATHUD::HideMainWidget);

				if (main_widget_type == EMainWidgetType::MWT_PauseMenu)
				{
					TArray<ESideWidgetType> showed_widgets_types;
					showed_side_widgets.GetKeys(showed_widgets_types);
					for (const auto type : showed_widgets_types)
						HideSideWidget(type);
					twidget->OnNeedToRemove.AddLambda([showed_widgets_types, this]()
					{
						for (const auto type : showed_widgets_types)
						{
							if (type != ESideWidgetType::SWT_MiniMap)
								ShowSideWidget(type, 1);
							else
								ShowSideWidget(type);
						}
					});
				}
			}
				
			return showed_main_widget;
		}
	}
	return nullptr;
}

UUserWidget * ATHUD::ShowSideWidget(const ESideWidgetType side_widget_type, const int32 ZOrder)
{
	HideSideWidget(side_widget_type);
	if (! SideWidgetClasses.Contains(side_widget_type))
		return nullptr;
	
	if (const auto new_side_class = SideWidgetClasses[side_widget_type])
	{
		showed_side_widgets.Add(side_widget_type, CreateWidget<UUserWidget>(GetWorld(), new_side_class));
		if (const auto widget = *showed_side_widgets.Find(side_widget_type))
			widget->AddToViewport(ZOrder);
		
		if (side_widget_type == ESideWidgetType::SWT_PlayerState)
			SetupPlayerState();

		if (const auto widget = *showed_side_widgets.Find(side_widget_type))
			return widget;
	}
	return nullptr;
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

bool ATHUD::IsMainWidgetShown(const EMainWidgetType type) const
{
	return type == showed_main_widget_type;
}

bool ATHUD::IsSideWidgetShown(const ESideWidgetType type) const
{
	return showed_side_widgets.Contains(type);
}

TArray<ESideWidgetType> ATHUD::ShowedSideWidgetsTypes() const
{
	TArray<ESideWidgetType> types;
	showed_side_widgets.GetKeys(types);
	return types;
}

void ATHUD::HideAllSideWidgets()
{
	TArray<ESideWidgetType> keys;
	showed_side_widgets.GetKeys(keys);
	for (const auto type : keys)
		HideSideWidget(type);
}
