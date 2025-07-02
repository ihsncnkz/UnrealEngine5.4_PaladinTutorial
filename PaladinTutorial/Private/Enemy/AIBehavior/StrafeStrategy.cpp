#include "Enemy/AIBehavior/StrafeStrategy.h"
#include "Enemy/EnemyAIController.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"

UStrafeStrategy::UStrafeStrategy()
{
}

void UStrafeStrategy::Execute(AEnemy* Enemy)
{
	// Get NavMesh Data
	FNavAgentProperties NavAgentProperties;
	const ANavigationData* NavData = UNavigationSystemV1::GetCurrent(Enemy->GetWorld())->GetNavDataForProps(NavAgentProperties);

	if (NavData != nullptr)
	{
		FNavLocation ResultLocation;

		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy->GetWorld());

		if (NavSystem)
		{
			// Get current rotation of actor
			// Create a 180 degree rotated rotator
			// Convert the rotator a vector
			const FRotator Rotation = Enemy->GetActorRotation();
			const FRotator TargetRotation(0, Rotation.Yaw + 180.f, 0);
			const FVector Dir = TargetRotation.Vector();

			// Generate a destination poin in the opposite direction
			FVector OppositeDirection = Enemy->GetActorLocation() + (Dir * 500.f);

			if (NavSystem->GetRandomPointInNavigableRadius(OppositeDirection, 500.f, ResultLocation))
			{
				// Get AI Controller move to location
				AEnemyAIController* AIController = Cast<AEnemyAIController>(Enemy->GetController());
				if (AIController != nullptr)
				{
					AIController->MoveToLocation(ResultLocation.Location);
				}
			}
		}
	}
}

bool UStrafeStrategy::HasReachedDestination(AEnemy* Enemy)
{ 
	AEnemyAIController* AIController = Cast<AEnemyAIController>(Enemy->GetController());
	if (AIController != nullptr)
	{
		EPathFollowingStatus::Type Status = AIController->GetMoveStatus();
		if (Status == EPathFollowingStatus::Idle || EPathFollowingStatus::Waiting)
		{
			return true;
		}
	}
	return false;
}