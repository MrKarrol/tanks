// Fill out your copyright notice in the Description page of Project Settings.


#include "Objective.h"

#include "InteractableObject.h"
#include "LocationMarker.h"


UInteractionObjective::UInteractionObjective()
{
	Type = EObjectiveType::Interact;
}

void UInteractionObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	if (const auto InteractableObject = Cast<IInteractableObject>(Target))
	{
		InteractableObject->OnInteractionFinished.AddLambda([this, Character]
			(AActor * InteractableObject, AActor * ActorInteractedWithObject)
			{
				if (bCanBeCompleted && Character == ActorInteractedWithObject)
				{
					bIsCompleted = true;
					if (OnObjectiveCompleted.IsBound())
					{
						OnObjectiveCompleted.Broadcast(this);
					}
				}
			});
	}
}

//----------------------------------------------------------------------------------

ULocationObjective::ULocationObjective()
{
	Type = EObjectiveType::Location;
}

void ULocationObjective::ActivateObjective(AActor* Character)
{
	Super::ActivateObjective(Character);

	if (const auto LocMarker = Cast<ILocationMarker>(Marker))
	{
		LocMarker->OnLocationReached.AddLambda([this, Character](
			AActor * LocationMarker, AActor * OverlappedActor)
		{
			if (bCanBeCompleted && Character == OverlappedActor)
			{
				bIsCompleted = true;
				if (OnObjectiveCompleted.IsBound())
				{
					OnObjectiveCompleted.Broadcast(this);
				}
			}
		});
	}
}

