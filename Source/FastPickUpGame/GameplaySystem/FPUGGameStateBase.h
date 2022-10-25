// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPUGGameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	TArray<int32>& GetScoreInfo();

protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	TArray<int32> TeamScores;
	
};
