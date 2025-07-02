#pragma once
#include "ICombatStrategy.h"
#include "PatrolStrategy.generated.h"

class AEnemy;

UCLASS()
class UPatrolStrategy : public UObject, public ICombatStrategy
{
	GENERATED_BODY()

public:

	UPatrolStrategy();
	virtual void Execute(AEnemy* Enemy) override;
	bool HasReachedDestination(AEnemy* Enemy);
};