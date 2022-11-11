// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameModeBase.h"

#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

#include "FPUGGameStateBase.h"
#include "FastPickUpGame/InteractSystem/ItemSystem/FPUGWorldItemScore.h"
#include "FastPickUpGame/CharacterSystem/FPUGPickUpInterface.h"
#include "FastPickUpGame/InteractSystem/EnvironmentSystem/FPUGDoorTechnical.h"
#include "WorldPartition/WorldPartition.h"

void AFPUGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnItems();
}

void AFPUGGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	//Gettig info about singleplayer/multiplayer mode from options
	const FString ModeInfo = UGameplayStatics::ParseOption(Options, "IsSinglePlayer");

	bIsSinglePlayer = ModeInfo.ToBool();
}

//Deciding if player is allowed to join the match
void AFPUGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	//Not allowing player to join the match if it has started or if there is too many players in match
	if (GetNumPlayers() == 2 || HasMatchStarted)
	{
		ErrorMessage = "Too many players in game";
	}
}

void AFPUGGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	const int32 NewPlayerId = GetNumPlayers() - 1;

	//Setting up convenient player id 
	NewPlayer->PlayerState->SetPlayerId(NewPlayerId);

	const auto PlayerChar = NewPlayer->GetPawn();

	IFPUGPickUpInterface* PickUpInterface = Cast<IFPUGPickUpInterface>(PlayerChar);

	if (PickUpInterface)
	{
		PickUpInterface->SetItemIdToCollect(GetItemIdsInCurrentMatch()[NewPlayerId]);
	}

	HandleNewPlayerInMatch();
}

void AFPUGGameModeBase::HandleNewPlayerInMatch()
{
	const int32 PlayersNeededForMatchToStart = bIsSinglePlayer ? 1 : 2;

	if (!HasMatchStarted && GetNumPlayers() >= PlayersNeededForMatchToStart)
	{
		HasMatchStarted = true;

		GetWorld()->GetTimerManager().SetTimer(MatchTimer, this, &AFPUGGameModeBase::StartMatch, PreMatchTime, false);
	}
}

void AFPUGGameModeBase::StartMatch()
{
	if (!GetGameStateInternal())
	{
		return;
	}

	InitScore();

	GS->InitTimeRemain();

	GetWorld()->GetTimerManager().SetTimer(MatchTimer, this, &AFPUGGameModeBase::EndMatch, GS->GetTimeRemain());

	OpenTechDoors();
}

void AFPUGGameModeBase::EndMatch()
{
	GetWorld()->GetTimerManager().ClearTimer(MatchTimer);

	ChooseWinner();

	FTimerHandle RestartTimer;

	GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &AFPUGGameModeBase::RestartMatch, PostMatchTime);
}

void AFPUGGameModeBase::ChooseWinner()
{
	if (!GetGameStateInternal())
	{
		return;
	}

	int32 WinnerId;
	int32 Score = -1;
	bool bIsDraw = true;

	const auto& ScoreInfo = GS->GetScoreInfo();

	if (ScoreInfo[0] != ScoreInfo[1])
	{
		bIsDraw = false;
	}

	if (bIsDraw)
	{
		WinnerId = -1;
	}
	else
	{
		UKismetMathLibrary::MaxOfIntArray(ScoreInfo, WinnerId, Score);
	}

	GS->SetWinnerId(WinnerId);
}

//Spawn points are calling this function on BeginPlay
void AFPUGGameModeBase::InitNewSpawnPoint(AActor* SpawnPointToAdd)
{
	SpawnPoints.AddUnique(SpawnPointToAdd);
}

//Technical doors are calling this function on BeginPlay
void AFPUGGameModeBase::InitNewTechDoor(AFPUGDoorTechnical* DoorToAdd)
{
	TechDoors.AddUnique(DoorToAdd);
}

void AFPUGGameModeBase::InitScore()
{
	if (!GetGameStateInternal())
	{
		return;
	}

	TArray<int32>& TeamsInfo = GS->GetScoreInfo();

	if (TeamsInfo.IsEmpty())
	{
		TeamsInfo.SetNum(2);
	}

	if (bIsSinglePlayer)
	{
		//Creating random opponent score for single player
		TeamsInfo[1] = FMath::RandRange(6, 10);
	}

	GS->OnRep_TeamScores();
}

//Handling score after someone picked up a score item
void AFPUGGameModeBase::AddScoreToTeamById(const int32 TeamId, const int32 ScoreToAdd)
{
	if (!GetGameStateInternal())
	{
		return;
	}

	TArray<int32>& TeamsInfo = GS->GetScoreInfo();

	TeamsInfo[TeamId] += ScoreToAdd;

	GS->OnRep_TeamScores();
}

//Spawning items in world
void AFPUGGameModeBase::SpawnItems()
{
	if (!GetGameStateInternal() || !GS->GetItemsDT())
	{
		return;
	}

	ItemIdsInCurrentMatch = GetItemIdsInCurrentMatch();

	FString Context;

	for (const int32 CurrentItemIndex : ItemIdsInCurrentMatch)
	{
		for (size_t i = 0; i < MaxItemsOnMap / ItemIdsInCurrentMatch.Num(); i++)
		{
			const int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);

			FTransform Position = SpawnPoints[RandomIndex]->GetActorTransform();

			const auto SpawnedItem = GetWorld()->SpawnActor<AFPUGWorldItemScore>(AFPUGWorldItemScore::StaticClass(), Position);

			SpawnedItem->SetItemID(CurrentItemIndex);

			SpawnedItem->OnRep_ItemID();

			SpawnPoints.RemoveAt(RandomIndex, 1, true);

			//If no more spawn points - end spawn 
			if (SpawnPoints.Num() == 0)
			{
				return;
			}
		}
	}
}

//Returns Item Ids chosen for current match, if array is empty - chooses items
TArray<int32> AFPUGGameModeBase::GetItemIdsInCurrentMatch()
{
	if (ItemIdsInCurrentMatch.IsEmpty())
	{
		do
		{
			//TODO: remove magic numbers
			int32 RandomId = FMath::RandRange(1, 10);

			ItemIdsInCurrentMatch.AddUnique(RandomId);

		} while (ItemIdsInCurrentMatch.Num() < 4);

		return ItemIdsInCurrentMatch;
	}

	return ItemIdsInCurrentMatch;
}

void AFPUGGameModeBase::RestartMatch() const
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	//TODO: remove hard code path
	const FString MapPath = "/Game/01_Levels/ThirdPersonMap";

	GetWorld()->ServerTravel(MapPath);
}

//Openning all Technical doors to let players out of start location
void AFPUGGameModeBase::OpenTechDoors()
{
	for (const auto CurrentDoor : TechDoors)
	{
		CurrentDoor->ToggleDoorState();
	}
}

AFPUGGameStateBase* AFPUGGameModeBase::GetGameStateInternal()
{
	if (!GS)
	{
		const auto World = GetWorld();

		if (World)
		{
			GS = GetGameState<AFPUGGameStateBase>();
		}
	}

	return GS;
}


