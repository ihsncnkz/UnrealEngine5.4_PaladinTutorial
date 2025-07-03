

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyProjectile.generated.h"

// Declaration
class UStaticMeshComponent;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class PALADINTUTORIAL_API AEnemyProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyProjectile();

	UFUNCTION()
	void OnProjectileOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Destroy actor when timer is complete
	void DestroyProjectile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Sphere component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	USphereComponent* SphereComponent;

	// Projectile component
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	UStaticMeshComponent* ProjectileMesh;

	// Projectile movement
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components", meta = (AllowPrivateAccess = true))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float ProjectileDamage;

	// Projectile life
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = true))
	float ProjectLife;

	// Setup projectile timer
	FTimerHandle ProjectileTimer;
	
};
