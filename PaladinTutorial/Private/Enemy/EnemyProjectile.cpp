#include "Enemy/EnemyProjectile.h"
#include "PaladinCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AEnemyProjectile::AEnemyProjectile() :
	ProjectileDamage(10.f),
	ProjectLife(5.f)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	SetRootComponent(SphereComponent);

	ProjectileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh"));
	ProjectileMesh->SetupAttachment(SphereComponent);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 3000;
	ProjectileMovement->MaxSpeed = 3000;
}

void AEnemyProjectile::OnProjectileOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checl if it hits the player
	if (OtherActor == nullptr) return;

	auto Character = Cast<APaladinCharacter>(OtherActor);

	if (Character)
	{
		UGameplayStatics::ApplyDamage(
			Character,
			ProjectileDamage,
			nullptr,
			this,
			UDamageType::StaticClass());
	}
	else
	{
		GetWorldTimerManager().SetTimer(ProjectileTimer, this, &AEnemyProjectile::DestroyProjectile, ProjectLife);
	}

}

void AEnemyProjectile::DestroyProjectile()
{
	Destroy();
}

// Called when the game starts or when spawned
void AEnemyProjectile::BeginPlay()
{
	Super::BeginPlay();

	// Bind function to overlap events for collision boxes
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyProjectile::OnProjectileOverlap);
}

// Called every frame
void AEnemyProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


