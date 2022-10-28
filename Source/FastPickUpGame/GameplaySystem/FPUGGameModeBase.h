// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPUGGameModeBase.generated.h"

class AFPUGGameStateBase;
class UDataTable;

UCLASS()
class FASTPICKUPGAME_API AFPUGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;
	
public:

	void InitNewSpawnPoint(AActor* SpawnPointToAdd);

	void AddScoreToTeamById(int32 TeamId, int32 ScoreToAdd);

protected:

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	AFPUGGameStateBase* GetGameStateInternal();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 MaxItemsOnMap = 40.f;

private:

	void UpdateMatchTimer();

	void EndMatch();

	void SpawnItems();

	TArray<int32> GetItemIdsInCurrentMatch();

private:

	FTimerHandle MatchTimer;

	AFPUGGameStateBase* GS;

	TArray<AActor*> SpawnPoints;

	TArray<int32> ItemIdsInCurrentMatch;

};
