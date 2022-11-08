// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPUGGameModeBase.generated.h"

class AFPUGGameStateBase;
class UDataTable;
class AFPUGDoorTechnical;

UCLASS()
class FASTPICKUPGAME_API AFPUGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;
	
public:

	void InitNewSpawnPoint(AActor* SpawnPointToAdd);

	void InitNewTechDoor(AFPUGDoorTechnical* DoorToAdd);

	void AddScoreToTeamById(int32 TeamId, int32 ScoreToAdd);

protected:

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

	AFPUGGameStateBase* GetGameStateInternal();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 MaxItemsOnMap = 40.f;

private:

	void UpdateMatchTimer();

	void EndMatch();

	void SpawnItems();

	TArray<int32> GetItemIdsInCurrentMatch();

	void RestartMatch();

	void HandleNewPlayerInMatch();

	void StartMatch();

	void OpenTechDoors();

private:

	FTimerHandle MatchTimer;

	UPROPERTY()
	AFPUGGameStateBase* GS;

	UPROPERTY()
	TArray<AActor*> SpawnPoints;

	UPROPERTY()
	TArray<AFPUGDoorTechnical*> TechDoors;

	TArray<int32> ItemIdsInCurrentMatch;

	bool HasMatchStarted = false;

};
