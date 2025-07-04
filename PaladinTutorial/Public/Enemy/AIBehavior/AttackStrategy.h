#pragma once


#include "AITypes.h"
#include "ICombatStrategy.h"
#include "Navigation/PathFollowingComponent.h"
#include "AttackStrategy.generated.h"

// Declarations
class AEnemy;
class APaladinCharacter;

UCLASS()

class UAttackStrategy : public UObject, public ICombatStrategy
{
	GENERATED_BODY()

public:
	UAttackStrategy();
	virtual void Execute(AEnemy* Enemy) override;
	virtual void OnMoveCompleted(FAIRequestID, const FPathFollowingResult& Result, AEnemy* Enemy, APaladinCharacter* PaladinCharacter);

private:
	float PlayerEnemyDistance(AEnemy* Enemy, APaladinCharacter* PaladinCharacter);
};