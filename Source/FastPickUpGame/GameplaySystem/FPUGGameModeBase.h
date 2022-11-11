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

	
public:

	void InitNewSpawnPoint(AActor* SpawnPointToAdd);

	void InitNewTechDoor(AFPUGDoorTechnical* DoorToAdd);

	void AddScoreToTeamById(const int32 TeamId, const int32 ScoreToAdd);

	void InitScore();

protected:

	void BeginPlay() override;

	void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage) override;

private:

	AFPUGGameStateBase* GetGameStateInternal();

	void EndMatch();

	void SpawnItems();

	TArray<int32> GetItemIdsInCurrentMatch();

	void RestartMatch() const;

	void HandleNewPlayerInMatch();

	void StartMatch();

	void OpenTechDoors();

	void ChooseWinner();

protected:

	//Number of total items that will be spawn in world at the start of a match
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 MaxItemsOnMap = 40;

	//Time before match starts, but after every player is ready to start
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 PreMatchTime = 5;

	//Time after match ends, but before restart
	UPROPERTY(EditDefaultsOnly, Category = "Game Rules")
	int32 PostMatchTime = 5;

private:

	FTimerHandle MatchTimer;

	//List of item Ids that was randomly chosen for current match.
	TArray<int32> ItemIdsInCurrentMatch;

	bool HasMatchStarted = false;

	bool bIsSinglePlayer = false;

	UPROPERTY()
	AFPUGGameStateBase* GS;

	//List of spawn points for world items
	UPROPERTY()
	TArray<AActor*> SpawnPoints;

	//Doors that should open on the start of a match
	UPROPERTY()
	TArray<AFPUGDoorTechnical*> TechDoors;
};
