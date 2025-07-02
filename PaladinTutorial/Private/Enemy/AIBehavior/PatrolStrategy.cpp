#include "Enemy/AIBehavior/PatrolStrategy.h"
#include "Enemy/EnemyAIController.h"
#include "NavigationSystem.h"
#include "AIController.h"
#include "Math/UnrealMathUtility.h"
#include "Navigation/PathFollowingComponent.h"

UPatrolStrategy::UPatrolStrategy()
{
}

void UPatrolStrategy::Execute(AEnemy* Enemy)
{
	FNavAgentProperties NavAgentProps;
	const ANavigationData* NavData = UNavigationSystemV1::GetCurrent(Enemy->GetWorld())->GetNavDataForProps(NavAgentProps);

	if (NavData != nullptr)
	{
		// Placeholder for out result
		FNavLocation ResultLocation;

		// Get Random paoint in radius
		UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(Enemy->GetWorld());
		if (NavSystem && NavSystem->GetRandomReachablePointInRadius(Enemy->GetActorLocation(), 800.f, ResultLocation))
		{
			// Use the ai controller to move to the result location
			AEnemyAIController* AIController = Cast<AEnemyAIController>(Enemy->GetController());

			// Check if we got controller and move to the point
			if (AIController != nullptr)
			{
				AIController->MoveToLocation(ResultLocation.Location);
			}
		}
	}
}

bool UPatrolStrategy::HasReachedDestination(AEnemy* Enemy)
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
