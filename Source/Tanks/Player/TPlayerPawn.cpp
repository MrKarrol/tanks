#include "TPlayerPawn.h"

#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Tanks/Components/THealthComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Tanks/Components/TInventoryComponent.h"
#include "Tanks/Components/TEquipInventoryComponent.h"
#include "Tanks/Components/TInventoryManagerComponent.h"
#include "Tanks/Guns/TGun.h"
#include "InteractionComponent.h"
#include "Quest.h"
#include "QuestListComponent.h"
#include "Blueprint/UserWidget.h"
#include "QuestDialog.h"
#include "QuestList.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"


ATPlayerPawn::ATPlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(BodyMeshComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	SpringArmThirdViewComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmThirdViewComponent");
	SpringArmThirdViewComponent->SetupAttachment(BodyMeshComponent);

	CameraThirdViewComponent = CreateDefaultSubobject<UCameraComponent>("CameraThirdViewComponent");
	CameraThirdViewComponent->SetupAttachment(SpringArmThirdViewComponent);
	CameraThirdViewComponent->Deactivate();

	InventoryComponent = CreateDefaultSubobject<UTInventoryComponent>("InventoryComponent");
	EquipmentInventoryComponent = 
		CreateDefaultSubobject<UTEquipInventoryComponent>("EquipInventory");
	InventoryManagerComponent = CreateDefaultSubobject<UTInventoryManagerComponent>("InventoryManagerComponent");

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>("InteractionComponent");
	InteractionComponent->SetupAttachment(BodyMeshComponent);

	QuestListComponent = CreateDefaultSubobject<UQuestListComponent>("QuestListComponent");
}

void ATPlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	if (const auto gun = GetCurrentGun())
	{
		gun->OnShotDelegate.AddUObject(this, &ATPlayerPawn::OnShot);
		gun->OnGetScoreDelegate.BindUObject(this, &ATPlayerPawn::TakeScore);
	}

	InventoryManagerComponent->Init(InventoryComponent);
	InventoryManagerComponent->InitEquipment(EquipmentInventoryComponent);
}

void ATPlayerPawn::SwapGuns()
{
	Super::SwapGuns();

	if (const auto gun = GetCurrentGun())
		DefineCameraView(gun);
}

void ATPlayerPawn::AddTurretHelper(ATEnemyTurret* helper)
{
	if (TurretHelpers.Num() >= MaxTurretHelpersCount)
		return;

	TurretHelpers.Emplace(helper);
}

bool ATPlayerPawn::CanSpawnTurretHelper() const
{
	return TurretHelpers.Num() < MaxTurretHelpersCount;
}

size_t ATPlayerPawn::TurretHelpersNumber() const
{
	return TurretHelpers.Num();
}

void ATPlayerPawn::ShowInventory()
{
	if (InventoryManagerComponent)
	{
		InventoryManagerComponent->ShowInventory();
		InventoryManagerComponent->ShowEquipmentInventory();
	}
}

void ATPlayerPawn::Interact()
{
	if (InteractionComponent)
		InteractionComponent->Interact();
}

void ATPlayerPawn::CalculateTopDownTurretRotation()
{
	if (const auto controller = Cast<APlayerController>(GetController()))
	{
		FHitResult result;
		controller->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Camera), true, result);
		if (result.Location.IsNearlyZero())
			return;
		FRotator new_turret_direction = UKismetMathLibrary::FindLookAtRotation(GetGunPivotAttach()->GetComponentLocation(), result.Location);
		new_turret_direction.Roll = GetGunPivotAttach()->GetComponentRotation().Roll;
		new_turret_direction.Pitch = GetGunPivotAttach()->GetComponentRotation().Pitch;
	
		AddTankTurretRotationInput(new_turret_direction);
	}
}

void ATPlayerPawn::CalculateThirdViewTurretRotation(float DeltaTime)
{
	FRotator summary_rotation_delta;
	if (FMath::IsNearlyZero(mCachedTurretUpDelta))
		mCurrentTurretUpSpeed = 0.f;
	else if (const auto gun = GetCurrentGun())
	{
		mCurrentTurretUpSpeed = FMath::FInterpConstantTo(mCurrentTurretUpSpeed, gun->TurretRotationSpeed * FMath::Abs(mCachedTurretUpDelta), DeltaTime, gun->TurretRotationAcceleration);
		const FRotator rotation_delta = FRotator(mCurrentTurretUpSpeed * DeltaTime * FMath::Sign(mCachedTurretUpDelta), 0.f, 0.f);
		summary_rotation_delta += rotation_delta;
		mCachedTurretUpDelta = 0.f;
	}

	if (FMath::IsNearlyZero(mCachedTurretRotationDelta))
		mCurrentTurretRotationSpeed = 0.f;
	else if (const auto gun = GetCurrentGun())
	{
		mCurrentTurretRotationSpeed = FMath::FInterpConstantTo(mCurrentTurretRotationSpeed, gun->TurretRotationSpeed * FMath::Abs(mCachedTurretRotationDelta), DeltaTime, gun->TurretRotationAcceleration);
		const FRotator rotation_delta = FRotator(0.f, mCurrentTurretRotationSpeed * DeltaTime * FMath::Sign(mCachedTurretRotationDelta), 0.f);
		summary_rotation_delta += rotation_delta;

		mCachedTurretRotationDelta = 0.f;
	}

	AddTankTurretRotationInput(GetGunPivotAttach()->GetComponentRotation() + summary_rotation_delta);
}

void ATPlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bInThirdPersonView)
		CalculateTopDownTurretRotation();
	else
		CalculateThirdViewTurretRotation(DeltaTime);
}

void ATPlayerPawn::Interact_Implementation(AActor* ActorInteractedWithObject)
{
	if (! ActorInteractedWithObject)
		return;
	
	const auto ActorQuestListComp = ActorInteractedWithObject->GetComponentByClass(
		UQuestListComponent::StaticClass());
	if (! ActorQuestListComp)
		return;

	const auto ActorQuestList = Cast<UQuestListComponent>(ActorQuestListComp);

	// here is limitations and quest logic

	TArray<AActor *> AttachedActors;
	GetAttachedActors(AttachedActors);
	bool HadQuestsAvailable = false;
	for (const auto Actor : AttachedActors)
	{
		if (const auto Quest = Cast<AQuest>(Actor))
		{
			if (Quest->IsAlreadyTaken() ||
				(Quest->GetPrerequisiteQuest() &&
				! Quest->GetPrerequisiteQuest()->IsCompleted()))
			{
				continue;
			}

			if (QuestDialogClass)
			{
				const auto QuestDialog = CreateWidget<UQuestDialog>(GetWorld(),
					QuestDialogClass);
				QuestDialog->Init(Quest);
				QuestDialog->OnQuestAccepted.BindUObject(
					ActorQuestList,
					&UQuestListComponent::AddQuest,
					Quest);
				QuestDialog->OnQuestQuited.BindLambda(
					[this, ActorInteractedWithObject]()
					{
						NotifyInteractionFinished(this, ActorInteractedWithObject);
					});
				QuestDialog->AddToViewport();
			}

			HadQuestsAvailable = true;
		}
	}

	if (! HadQuestsAvailable)
		NotifyInteractionFinished(this, ActorInteractedWithObject);
}

void ATPlayerPawn::ToggleQuestListVisibility()
{
	const auto PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (QuestList)
	{
		QuestList->RemoveFromParent();
		QuestList = nullptr;
	}
	else if (QuestListClass)
	{
		QuestList = CreateWidget<UQuestList>(GetWorld(), QuestListClass);
		QuestList->Init(QuestListComponent);
		QuestList->AddToViewport();
	}
}

void ATPlayerPawn::DefineCameraView(ATGun* gun)
{
	if (gun->NeedThirdView)
	{
		CameraComponent->Deactivate();
		CameraThirdViewComponent->Activate();
		bInThirdPersonView = true;
	}
	else
	{
		CameraThirdViewComponent->Deactivate();
		CameraComponent->Activate();
		bInThirdPersonView = false;
	}
}

void ATPlayerPawn::SetGun(TSubclassOf<ATGun> GunClass)
{
	Super::SetGun(GunClass);

	if (auto gun = GetCurrentGun())
	{
		gun->OnShotDelegate.AddUObject(this, &ATPlayerPawn::OnShot);
		gun->OnGetScoreDelegate.BindUObject(this, &ATPlayerPawn::TakeScore);
	}
}

void ATPlayerPawn::MoveForward(float AxisValue)
{
	AddTankMovementInput(AxisValue);
}

void ATPlayerPawn::MoveRight(float AxisValue)
{
	AddTankRotationInput(AxisValue);
}

void ATPlayerPawn::MoveTurretUp(float AxisValue)
{
	mCachedTurretUpDelta = FMath::Clamp(AxisValue, -1.f, 1.f);
}

void ATPlayerPawn::MoveTurretRight(float AxisValue)
{
	mCachedTurretRotationDelta = FMath::Clamp(AxisValue, -1.f, 1.f);
}

void ATPlayerPawn::TakeScore(float InScore)
{
	TotalScore += InScore;
	if (OnChangeScoreDelegate.IsBound())
		OnChangeScoreDelegate.Execute(FMath::CeilToInt(TotalScore));
}

void ATPlayerPawn::Heal(float HealthGain)
{
	HealthComponent->SetHealth(HealthComponent->GetHealth() + HealthGain);
}

void ATPlayerPawn::OnShot(ATGun*) const
{
	const auto controller = Cast<APlayerController>(GetController());
	const auto gun = GetCurrentGun();
	if (gun && gun->ShotCameraShake && controller)
	{
		controller->ClientStartCameraShake(gun->ShotCameraShake);
	}
}

void ATPlayerPawn::OnDie()
{
	OnPawnDied();
}
