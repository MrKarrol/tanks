// Fill out your copyright notice in the Description page of Project Settings.


#include "TTurretSpawnWidget.h"

#include "Kismet/GameplayStatics.h"
#include "Tanks/Player/TPlayerController.h"
#include "Components/RichTextBlock.h"
#include "Tanks/Enemies/TEnemyTurret.h"

void UTTurretSpawnWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (const auto player_controller = Cast<ATPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		player_controller->OnMouseButtonUp.AddUObject(this, &UTTurretSpawnWidget::OnMouseButtonUp);
		PlayerController = player_controller;
	}

	if (DraggedActorNameText)
	{
		const FText dragged_actor_text = FText::FromString(DraggedActorName);
		DraggedActorNameText->SetText(dragged_actor_text);
	}
}

void UTTurretSpawnWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (DraggedActor && PlayerController)
	{
		FHitResult HitResult;
		PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, HitResult);
		if (HitResult.Location.IsNearlyZero())
			return;

		GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Red, FString::Printf(TEXT("%s"), *HitResult.Location.ToString()));

		if (HitResult.Actor.Get())
		{
			DraggedActor->SetActorLocation(HitResult.Location);
		}
	}
}

FReply UTTurretSpawnWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(EKeys::LeftMouseButton))
	{
		// DraggedActor = GetWorld()->SpawnActor<AActor>(DraggedActorClass);
		const FTransform empty_transform;
		DraggedActor = GetWorld()->SpawnActorDeferred<AActor>(DraggedActorClass, empty_transform);
		if (const auto turret = Cast<ATEnemyTurret>(DraggedActor))
			turret->SetInitialGun(DraggedActorGunClass);
		DraggedActor->FinishSpawning(empty_transform);
	}
	return FReply::Handled();
}

void UTTurretSpawnWidget::OnMouseButtonUp()
{
	if (DraggedActor)
		DraggedActor = nullptr;
}
