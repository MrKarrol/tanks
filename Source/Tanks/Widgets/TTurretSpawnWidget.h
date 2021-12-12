// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TTurretSpawnWidget.generated.h"

class URichTextBlock;
/**
 * 
 */
UCLASS()
class TANKS_API UTTurretSpawnWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	void OnMouseButtonUp();
	
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> DraggedActorClass;

	UPROPERTY(EditAnywhere)
	FString DraggedActorName;

	UPROPERTY()
	AActor * DraggedActor;

	UPROPERTY()
	APlayerController * PlayerController;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *DraggedActorNameText;

};
