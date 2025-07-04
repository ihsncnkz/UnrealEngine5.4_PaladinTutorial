// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PALADINTUTORIAL_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleAnyWhere, Category = "SaveGame")
	float Health;

	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	FVector CheckpointLocation;
};
