// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPUGGameStateBase.generated.h"

class UDataTable;

UCLASS()
class FASTPICKUPGAME_API AFPUGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	TArray<int32>& GetScoreInfo();

	int32 GetTimeRemain() const;

	void SetTimeRemain(const int32 NewTime);

	UDataTable* GetItemsDT();

protected:

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	TArray<int32> TeamScores;

	UPROPERTY(EditDefaultsOnly, Category = "Time")
	int32 MatchTime = 30;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Time")
	int32 TimeRemain = MatchTime;

	UPROPERTY(EditDefaultsOnly, Category = "Items")
	UDataTable* ItemsDT;
	
};
