// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameModeBase.h"

#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

#include "FPUGGameStateBase.h"
#include "FastPickUpGame/InteractSystem/ItemSystem/FPUGWorldItemScore.h"
#include "FastPickUpGame/CharacterSystem/FPUGPickUpInterface.h"

void AFPUGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnItems();

	for (auto CurrentPlayer : GetGameStateInternal()->PlayerArray)

	//TODO: Remove first delay
	GetWorld()->GetTimerManager().SetTimer(MatchTimer, this, &AFPUGGameModeBase::UpdateMatchTimer, 1.f, true, 5.f);
}

void AFPUGGameModeBase::InitNewSpawnPoint(AActor* SpawnPointToAdd)
{
	SpawnPoints.AddUnique(SpawnPointToAdd);
}

void AFPUGGameModeBase::AddScoreToTeamById(int32 TeamId, int32 ScoreToAdd)
{
	auto CurrentGS = GetGameStateInternal();

	if (!CurrentGS)
	{
		return;
	}

	TArray<int32>& TeamsInfo = CurrentGS->GetScoreInfo();

	if (TeamsInfo.IsEmpty())
	{
		TeamsInfo.SetNum(GetNumPlayers());
	}

	TeamsInfo[TeamId] += ScoreToAdd;

	CurrentGS->OnRep_TeamScores();
}


void AFPUGGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	const int32 NewPlayerId = GetNumPlayers() - 1;

	NewPlayer->PlayerState->SetPlayerId(NewPlayerId);

	const auto PlayerChar = NewPlayer->GetPawn();

	IFPUGPickUpInterface* PickUpIterface = Cast<IFPUGPickUpInterface>(PlayerChar);

	if (PickUpIterface)
	{
		PickUpIterface->SetItemIdToCollect(GetItemIdsInCurrentMatch()[NewPlayerId]);
	}
}

void AFPUGGameModeBase::UpdateMatchTimer()
{
	auto CurrentGS = GetGameStateInternal();

	if (!CurrentGS)
	{
		return;
	}

	const int32 UpdatedTime = CurrentGS->GetTimeRemain() - 1;

	CurrentGS->SetTimeRemain(UpdatedTime);

	if (UpdatedTime <= 0)
	{
		EndMatch();
	}
}

void AFPUGGameModeBase::EndMatch()
{
	GetWorld()->GetTimerManager().ClearTimer(MatchTimer);

	//TPODO: End Game
}

void AFPUGGameModeBase::SpawnItems()
{
	if (!GetGameStateInternal() || !GS->GetItemsDT())
	{
		return;
	}

	SpawnPoints.Num();

	ItemIdsInCurrentMatch = GetItemIdsInCurrentMatch();

	FString Context;

	for (auto CurrentItemIndex : ItemIdsInCurrentMatch)
	{
		for (size_t i = 0; i < MaxItemsOnMap / 4; i++)
		{
			int32 RandomIndex = FMath::RandRange(0, SpawnPoints.Num() - 1);

			FTransform Position = SpawnPoints[RandomIndex]->GetActorTransform();

			auto SpawnedItem = GetWorld()->SpawnActor<AFPUGWorldItemScore>(AFPUGWorldItemScore::StaticClass(), Position);

			SpawnedItem->SetItemID(CurrentItemIndex);

			SpawnedItem->OnRep_ItemID();

			SpawnPoints.RemoveAt(RandomIndex, 1, true);

			if (SpawnPoints.Num() == 0)
			{
				return;
			}

		}
	}

	
}

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


AFPUGGameStateBase* AFPUGGameModeBase::GetGameStateInternal()
{
	if (!GS)
	{
		auto World = GetWorld();

		if (World)
		{
			GS = GetGameState<AFPUGGameStateBase>();
		}
	}

	return GS;
}


