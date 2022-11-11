// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "FPUGGameStateBase.generated.h"

class UDataTable;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnScoreUpdated, const TArray<int32>&, NewScore);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInitTime, int32, TimeLeft);

UCLASS()
class FASTPICKUPGAME_API AFPUGGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:

	TArray<int32>& GetScoreInfo();

	int32 GetTimeRemain() const;

	void SetTimeRemain(const int32 NewTime);

	void SetWinnerId(const int32 WinnerId);

	UDataTable* GetItemsDT();

	UFUNCTION(BlueprintCallable, Category = "Time")
	void InitTimeRemain();

	UFUNCTION()
	void OnRep_TeamScores();

	UFUNCTION()
	void OnRep_TimeRemain();

	UFUNCTION()
	void OnRep_WinnerPlayerId();

public:

	UPROPERTY(BlueprintAssignable)
	FOnScoreUpdated OnScoreUpdated;

	UPROPERTY(BlueprintAssignable)
	FOnInitTime OnInitTime;

protected:

	//Player Scores Info. 0 index - first player, 1 index - second
	UPROPERTY(ReplicatedUsing = "OnRep_TeamScores", BlueprintReadOnly, Category = "Score")
	TArray<int32> TeamScores;

	UPROPERTY(EditDefaultsOnly, Category = "Time")
	int32 MatchTime = 30;

	UPROPERTY(ReplicatedUsing = "OnRep_TimeRemain", BlueprintReadOnly, Category = "Time")
	int32 TimeRemain = -1;

	//Data table that contains info about Items.
	UPROPERTY(EditDefaultsOnly, Category = "Items")
	UDataTable* ItemsDT;

	UPROPERTY(ReplicatedUsing = "OnRep_WinnerPlayerId", BlueprintReadOnly, Category = "Score")
	int32 WinnerPlayerId = -2;
	
};
