// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tanks/Widgets/TWidget.h"
#include "PlayerStateWidget.generated.h"

class UProgressBar;
class URichTextBlock;

/**
 * 
 */
UCLASS()
class TANKS_API UPlayerStateWidget : public UTWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void SetMaxHealth(float max_health);
	void SetHealth(float health, float old_health) const;

	void SetFirstGunAmmo(int32 ammo_count);
	void SetSecondGunAmmo(int32 ammo_count);

	void SetFirstGunName(const FString &name);
	void SetSecondGunName(const FString &name);

	void SetScore(int32 score) const;
	void SetEnemiesCount(int32 enemies_count) const;

private:
	void SetupGunName(URichTextBlock *gun_name_text, const FString &name);
	void SetupGunAmmo(URichTextBlock *gun_ammo_text, const int32 ammo_count);

protected:
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	UProgressBar *HealthBar;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *ScoreText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *EnemiesText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *FirstGunNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *FirstGunAmmoText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *SecondGunNameText;

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	URichTextBlock *SecondGunAmmoText;

	UPROPERTY(EditAnywhere)
	FString EmptyGunSlotName = "Empty slot";

private:
	float m_max_health;

};
