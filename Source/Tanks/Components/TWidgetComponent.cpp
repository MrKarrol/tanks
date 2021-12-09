// Fill out your copyright notice in the Description page of Project Settings.


#include "TWidgetComponent.h"
#include "Tanks/Widgets/TWidget.h"

UTWidgetComponent::UTWidgetComponent()
{
	// Set common defaults when using widgets on Actors
	SetDrawAtDesiredSize(true);
	SetWidgetSpace(EWidgetSpace::World);
}


void UTWidgetComponent::InitWidget()
{
	// Base implementation creates the 'Widget' instance
	Super::InitWidget();

	SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	if (const auto WidgetInst = Cast<UTWidget>(GetWidget()))
	{
		WidgetInst->SetOwningActor(GetOwner());
	}
}