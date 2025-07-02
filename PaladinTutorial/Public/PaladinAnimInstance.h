// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PaladinAnimInstance.generated.h"

// Declarations
class APaladinCharacter;

/**
 * 
 */
UCLASS()
class PALADINTUTORIAL_API UPaladinAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// Used in animation blueprints
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);

	// We can initialize variable use them in blueprints
	virtual void NativeInitializeAnimation() override;

private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category="Movement", meta=(AllowPrivateAccess="true"))
	APaladinCharacter* PaladinCharacter;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Direction;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsBlocking;

public:

	FORCEINLINE bool GetIsBlocking() const
	{
		return bIsBlocking;
	}

	FORCEINLINE void SetIsBlocking(bool blocking)
	{
		bIsBlocking = blocking;
	}
};
