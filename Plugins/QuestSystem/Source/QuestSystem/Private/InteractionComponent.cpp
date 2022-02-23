// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "InteractableObject.h"
#include "Blueprint/UserWidget.h"


// Sets default values for this component's properties
UInteractionComponent::UInteractionComponent()
{
	ActorToInteract = nullptr;
	
	PrimaryComponentTick.bCanEverTick = true;
}

void UInteractionComponent::Interact()
{
	if (!ActorToInteract)
		return;

	IInteractableObject::Execute_Interact(ActorToInteract, GetOwner());
}


// Called when the game starts
void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this,
		&UInteractionComponent::OnInteractionCompBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this,
		&UInteractionComponent::OnInteractionCompEndOverlap);
}

void UInteractionComponent::OnInteractionCompBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (! (OtherActor && OtherActor->Implements<UInteractableObject>()) || OtherActor == GetOwner())
		return;

	ActorToInteract = OtherActor;
	if (!InteractMessage)
	{
		if (InteractMessageClass)
		{
			InteractMessage = CreateWidget<UUserWidget>(GetWorld(),
				InteractMessageClass);
		}
	}
	if (InteractMessage && !InteractMessage->IsInViewport())
		InteractMessage->AddToViewport();
}

void UInteractionComponent::OnInteractionCompEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ActorToInteract = nullptr;
	if (InteractMessage)
		InteractMessage->RemoveFromViewport();
}


// Called every frame
void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

