// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitInterface.h"
#include "AIBehavior/AttackStrategy.h"
#include "AIBehavior/PatrolStrategy.h"
#include "AIBehavior/StrafeStrategy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "NiagaraFunctionLibrary.h"
#include "Enemy.generated.h"

// List of AI States
UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle   UMETA(DisplayName = "Idle"),
	Patrol UMETA(DisplayName = "Patrol"),
	Attack UMETA(DisplayName = "Attack"),
	Combat UMETA(DisplayName = "Combat"),
	Strafe UMETA(DisplayName = "Strafe"),
	Dead   UMETA(DisplayName = "Dead"),
};

// Declaration
class UAnimMontage;
class AEnemyAIController;
class USoundCue;
class UNiagaraSystem;

UCLASS()
class PALADINTUTORIAL_API AEnemy : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

	// Enter and Exit Combat
	void EnterCombat();
	void ExitCombat();

	void MeleeRangeAttack();
	void ResetMeleeRangeAttack();

	// Spawn projects for enemy projetile
	void SpawnProjectile();

	// Activate and deactivate weapon boxes
	virtual void ActivateRightWeapon();
	virtual void DeactivateRightWeapon();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Override hit interface
	virtual void HitInterface_Implementation(FHitResult HitResult) override;
	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// used for AI state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	EAIState CurrentState;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Enemy AI Controller
	UPROPERTY()
	AEnemyAIController* EnemyAIController;

	// Right Weapon overlap
	UFUNCTION()
	void OnRightWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintCallable)
	void ResetAttack();

	FName GetAttackSectionName(int32 SectionCount);

	UFUNCTION(BlueprintImplementableEvent)
	void EnemyDeath();

private:
	// Combat strategy logic
	TWeakObjectPtr<UPatrolStrategy> PatrolStrategy;
	TWeakObjectPtr<UAttackStrategy> AttackStrategy;
	TWeakObjectPtr<UStrafeStrategy> StrafeStrategy;

	// This is used in tick for patrolling
	bool bIsWaiting;

	FTimerHandle PatrolDelayTimer;

	void EnemyPatrol();
	void EnemyAttack();
	void EnemyStrafe();

	// Timer Attack handle
	FTimerHandle TimerAttack;

	// Right Weapon Collision
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName RightWeaponSocketName = "RightWeaponSocket";

	// Get blueprint of projectile. Set this in Enemy blueprint
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AEnemyProjectile> ProjectileBP;

	// base damege
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float AcceptanceRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float StrafeDelayTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundCue* ImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* ImpactNiagara;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FName ImpactBoneLocation;

public:
	// Getters end Setters
	FORCEINLINE float GetAttackRange() const { return AttackRange; }
	FORCEINLINE float GetAcceptanceRange() const { return AcceptanceRange; }

	FORCEINLINE float SetAttackRange(const float AttackRangeSet)
	{
		AttackRange = AttackRangeSet;
		return AttackRange;
	}
};
