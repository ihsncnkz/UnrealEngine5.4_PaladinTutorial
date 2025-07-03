// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "PaladinCharacter.h"
#include "Enemy/EnemyAIController.h"
#include "Enemy/EnemyProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

// Sets default values
AEnemy::AEnemy() : 
	BaseDamage(5.f), 
	Health(100.f), 
	MaxHealth(100.f),
	AttackRange(300.f),
	AcceptanceRange(200.f)
{
	// Right weapon collision boz
	RightWeaponCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Right Weapon Box"));
	RightWeaponCollision->SetupAttachment(GetMesh(), RightWeaponSocketName);
}

void AEnemy::EnterCombat()
{
	CurrentState = EAIState::Attack;
}

void AEnemy::ExitCombat()
{
	bIsWaiting = false;
	CurrentState = EAIState::Patrol;
	if (EnemyAIController != nullptr)
	{
		EnemyAIController->ClearFocus(EAIFocusPriority::Gameplay);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyAIController is null in ExitCombat"));
	}
}

void AEnemy::ActivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemy::DeactivateRightWeapon()
{
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	// Cam Ememy patrol
	CurrentState = EAIState::Patrol;

	// Setup Enemy Controller
	EnemyAIController = Cast<AEnemyAIController>(GetController());

	// Bind function to overlao for weapon box
	RightWeaponCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::OnRightWeaponOverlap);

	// Setup right weapon collision
	RightWeaponCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RightWeaponCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	RightWeaponCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	RightWeaponCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
}

void AEnemy::OnRightWeaponOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr) return;

	auto Character = Cast<APaladinCharacter>(OtherActor);

	if (Character)
	{
		UGameplayStatics::ApplyDamage(
			Character,
			BaseDamage,
			EnemyAIController,
			this,
			UDamageType::StaticClass());
	}
}

void AEnemy::MeleeAttack()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (AnimInstance && AttackMontage)
	{
		if (!AnimInstance->Montage_IsPlaying(AttackMontage))
		{
			// Get number of montage sections
			int32 const SectionCount = AttackMontage->CompositeSections.Num();

			// Get random animation to play
			// Get section index and playtime to use for timer
			FName const SectionName = GetAttackSectionName(SectionCount);
			int32 const SectionIndex = AttackMontage->GetSectionIndex(SectionName);
			float const SectionLenght = AttackMontage->GetSectionLength(SectionIndex);

			// Play montage section
			AnimInstance->Montage_Play(AttackMontage, 0.5f);
			AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
			//GetWorldTimerManager().SetTimer(TimerAttack, this, &AEnemy::ResetAttack, SectionLenght, false);

			// Call reset melee attack
			FTimerHandle TimerResetAttack;
			GetWorldTimerManager().SetTimer(TimerResetAttack, this, &AEnemy::ResetMeleeAttack, SectionLenght, false);
		}
	}
}

void AEnemy::ResetMeleeAttack()
{
	float RandomChance = FMath::FRand();
	if (RandomChance <= 0.3f)
	{
		CurrentState = EAIState::Strafe;
	}
}

void AEnemy::SpawnProjectile()
{
	// Get socket transform
	FTransform SocketTransform = GetMesh()->GetSocketTransform("ProjectileSocket");

	// Set spawn parameters
	FActorSpawnParameters SpawnParameters;

	// Spawn the projectile
	AEnemyProjectile* Projectile = GetWorld()->SpawnActor<AEnemyProjectile>(ProjectileBP, SocketTransform, SpawnParameters);
}

void AEnemy::ResetAttack()
{
	//MeleeAttack();
	// Reset enemy state
	// random count1-4
	// if < 2run stafe code or attack 

}

FName AEnemy::GetAttackSectionName(int32 SectionCount)
{
	FName SectionName;

	// Get random section in montage
	const int32 Section{ FMath::RandRange(1, SectionCount) };

	switch (Section)
	{
	case 1:
		SectionName = FName("Attack1");
		break;
	case 2:
		SectionName = FName("Attack2");
		break;
	case 3:
		SectionName = FName("Attack3");
		break;
	case 4:
		SectionName = FName("Attack4");
		break;
	}

	return SectionName;
}

void AEnemy::EnemyPatrol()
{
	if (PatrolStrategy.IsValid())
	{
		PatrolStrategy->Execute(this);
	}
	else
	{
		PatrolStrategy = NewObject<UPatrolStrategy>();
		PatrolStrategy->Execute(this);
	}
	bIsWaiting = false;
}

void AEnemy::EnemyAttack()
{
	if (AttackStrategy.IsValid())
	{
		AttackStrategy->Execute(this);
	}
	else
	{
		AttackStrategy = NewObject<UAttackStrategy>();
		AttackStrategy->Execute(this);
	}
	bIsWaiting = false;
}

void AEnemy::EnemyStrafe()
{
	bIsWaiting = false;
	CurrentState = EAIState::Attack;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	switch (CurrentState)
	{
	case EAIState::Attack:
	{
		if (!bIsWaiting)
		{
			bIsWaiting = true;
			float AttackDelay = FMath::RandRange(0.75f, 2.f);
			FTimerHandle AttackDelayTimer;
			GetWorldTimerManager().SetTimer(AttackDelayTimer, this, &AEnemy::EnemyAttack, AttackDelay, false);
		}
		break;
	}
	case EAIState::Strafe:
	{
		if (StrafeStrategy->HasReachedDestination(this) && !bIsWaiting)
		{
			bIsWaiting = true;
			if (StrafeStrategy.IsValid())
			{
				StrafeStrategy->Execute(this);
			}
			else
			{
				StrafeStrategy = NewObject<UStrafeStrategy>();
				StrafeStrategy->Execute(this);
			}
			float StrafeDelay = FMath::RandRange(1.f, StrafeDelayTime);
			FTimerHandle StrafeDelayTimer;
			GetWorldTimerManager().SetTimer(StrafeDelayTimer, this, &AEnemy::EnemyStrafe, StrafeDelay, false);
		}
		break;
	}
	case EAIState::Patrol:
	{
		if (PatrolStrategy->HasReachedDestination(this) && !bIsWaiting)
		{
			bIsWaiting = true;
			float PatrolDelay = FMath::RandRange(1.0f, 5.0f);
			GetWorldTimerManager().SetTimer(PatrolDelayTimer, this, &AEnemy::EnemyPatrol, PatrolDelay, false);
		}
		break;
	}
	}
}

void AEnemy::HitInterface_Implementation(FHitResult HitResult)
{
	// impact sound
	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	// impact niagara
	const FVector SpawnLocation = GetMesh()->GetBoneLocation(ImpactBoneLocation, EBoneSpaces::WorldSpace);
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), ImpactNiagara, SpawnLocation, GetActorRotation());
	// hit montage
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
		// call blueprint function to play death montage and clean things up
		EnemyAIController->UnPossess();
		SetActorEnableCollision(false);
		EnemyDeath();
	}
	else
	{
		Health -= DamageAmount;
	}

	return DamageAmount;
}
