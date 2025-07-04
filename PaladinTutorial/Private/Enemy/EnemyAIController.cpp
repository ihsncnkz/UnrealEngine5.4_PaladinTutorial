// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/EnemyAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "PaladinCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	SetupPerceptionSystem();
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	// Add a small delay to ensure the pawn is fully initialized
	FTimerHandle TimerPawnInit;
	GetWorld()->GetTimerManager().SetTimer(TimerPawnInit, this, &AEnemyAIController::SetupControlledPawn, 0.1f, false);
}

void AEnemyAIController::SetupControlledPawn()
{
	AEnemy* Enemy = Cast<AEnemy>(GetPawn());
	if (Enemy != nullptr)
	{
		ControlledEnemy = Enemy;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("AEnemyAIController::SetupControlledPawn: No controlled pawn found"));
	}
}

void AEnemyAIController::SetupPerceptionSystem()
{
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	if (SightConfig)
	{
		PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component"));
		SetPerceptionComponent(*PerceptionComponent);

		SightConfig->SightRadius = 1000.f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.f;
		SightConfig->PeripheralVisionAngleDegrees = 120.f;
		SightConfig->SetMaxAge(5.f);
		SightConfig->AutoSuccessRangeFromLastSeenLocation = 1050.f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}
}

void AEnemyAIController::OnTargetDetected(AActor* Actor, FAIStimulus const Stimulus)
{
	if (auto* const PaladinCharacter = Cast<APaladinCharacter>(Actor))
	{
		if (Stimulus.IsActive())
		{
			if (ControlledEnemy != nullptr)
			{
				ControlledEnemy->EnterCombat();
			}
		}
		else
		{
			if (ControlledEnemy != nullptr)
			{
				ControlledEnemy->ExitCombat();
			}
		}
	}
}
