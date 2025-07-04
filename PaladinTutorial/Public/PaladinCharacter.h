// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "MotionWarpingComponent.h"
#include "MotionWarpingClasses.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PaladinCharacter.generated.h"

UENUM(BlueprintType)
enum class EPlayerState : uint8
{
	Ready	    UMETA(DisplayName = "Ready"),
	NotReady    UMETA(DisplayName = "Not Ready"),
	Attacking   UMETA(DisplayName = "Attacking"),
	BlockDodge  UMETA(DisplayName = "BlockDodge"),
	Attack		UMETA(DisplayName = "Attack"),
	Stunned		UMETA(DisplayName = "Stunned"),
	Dead		UMETA(DisplayName = "Dead"),
};

// Declaration
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class UAnimMontage;
class USoundCue;
class UMotionWarpingComponent;

UCLASS()
class PALADINTUTORIAL_API APaladinCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APaladinCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Motion Warping
	UPROPERTY(EditAnywhere, Category = "Combat")
	UMotionWarpingComponent* MotionWarpingComponent;

	void MotionWarpAttack(float AttackDistance, FName MotionWarpName);
	void ResetWarpAttack();

	// Input Actions
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* BasicAttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* HeavyAttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* SpinAttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* JumpAttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* RunAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* BlockAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* DodgeBackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* DodgeLeftAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* DodgeRightAction;

	// Walk Speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed;

	// Run Speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
	float RunSpeed;


	// Movement
	void Move(const FInputActionValue& InputValue);
	void Look(const FInputActionValue& InputValue);
	void Jump();
	void Running();
	void StopRunning();

	// Attack
	void BasicAttack();
	void HeavyAttack();
	void SpinAttack();
	void JumpAttack();

	// Dodge
	void DodgeBack();
	void DodgeLeft();
	void DodgeRight();

	// Blocking
	void StartBlocking();
	void StopBlocking();
	void ResetDodgeRoll();

	// Handle logic after player died
	UFUNCTION(BlueprintImplementableEvent)
	void DeathOfPlayer();

	void AnimMontagePlay(UAnimMontage* MontageToPlay, FName SectionName = "Default", float PlayRate = 1.0f);

	UFUNCTION()
	void OnRightWeaponOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

public:
	// Save and load player data
	UFUNCTION(BlueprintCallable, Category = "Saved Data")
	void SavePlayerData();
	void LoadPlayerData();

	// Current State
	EPlayerState CurrentState;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void ActivateRightWeapon();
	virtual void DeactivateRightWeapon();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

private:
	// Timers
	FTimerHandle TimerDodgeRoll;

	// Last checkpoint location
	FVector CheckpointLocation;

	// Spring arm component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArmComponent;

	// Follow camera component
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCameraComponent;

	// Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Montage", meta = (AllowPrivateAccess = "true"))
	UAnimMontage* DodgeMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightWeaponCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float BaseDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundCue* BodyImpactSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	USoundCue* ShieldImpactSound;
	
	bool PlayerFacingActor(AActor* FacingActor);

	// AI percetion system
	UAIPerceptionStimuliSourceComponent* StimuliSource;
	void SetupStimulusSource();
};
