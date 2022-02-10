// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"


void AInteractableActor::OnInteractionFinished(AActor * ActorInteractedWithObject)
{
	NotifyInteractionFinished(this, ActorInteractedWithObject);
}

