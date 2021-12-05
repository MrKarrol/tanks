// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStateWidget.h"

#include "Components/ProgressBar.h"
#include "Components/RichTextBlock.h"


void UPlayerStateWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetupGunName(FirstGunNameText, "");
	SetupGunName(SecondGunNameText, "");
}

void UPlayerStateWidget::SetMaxHealth(float max_health)
{
	m_max_health = max_health;
}

void UPlayerStateWidget::SetHealth(float health, float) const
{
	if (HealthBar)
		HealthBar->SetPercent(health / m_max_health);
}

void UPlayerStateWidget::SetFirstGunAmmo(int32 ammo_count)
{
	if (FirstGunNameText->GetText().ToString() != EmptyGunSlotName)
		SetupGunAmmo(FirstGunAmmoText, ammo_count);
}

void UPlayerStateWidget::SetSecondGunAmmo(int32 ammo_count)
{
	if (SecondGunNameText->GetText().ToString() != EmptyGunSlotName)
		SetupGunAmmo(SecondGunAmmoText, ammo_count);
}

void UPlayerStateWidget::SetFirstGunName(const FString& name)
{
	SetupGunName(FirstGunNameText, name);
}

void UPlayerStateWidget::SetSecondGunName(const FString& name)
{
	SetupGunName(SecondGunNameText, name);
}

void UPlayerStateWidget::SetScore(int32 score) const
{
	if(ScoreText)
	{
		const FText score_text = FText::FromString(FString("Score: ") + FString::FromInt(score));
		ScoreText->SetText(score_text);
	}
}

void UPlayerStateWidget::SetEnemiesCount(int32 enemies_count) const
{
	if (EnemiesText)
	{
		const FText enemies_text = FText::FromString(FString("Enemies: ") + FString::FromInt(enemies_count));
		EnemiesText->SetText(enemies_text);
	}
}

void UPlayerStateWidget::SetupGunName(URichTextBlock* gun_name_text, const FString& name)
{
	if (gun_name_text)
	{
		if (name.IsEmpty())
			gun_name_text->SetText(FText::FromString(EmptyGunSlotName));
		else
			gun_name_text->SetText(FText::FromString(name + FString(": ")));
	}
}

void UPlayerStateWidget::SetupGunAmmo(URichTextBlock* gun_ammo_text, const int32 ammo_count)
{
	if (gun_ammo_text)
	{
		const FText new_ammo_text = FText::FromString(FString::FromInt(ammo_count));
		gun_ammo_text->SetText(new_ammo_text);
	}
}
