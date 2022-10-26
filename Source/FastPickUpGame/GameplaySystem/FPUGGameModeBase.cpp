// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameModeBase.h"

#include "GameFramework/PlayerState.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "Engine/DataTable.h"
#include "Kismet/GameplayStatics.h"

#include "FPUGGameStateBase.h"
#include "FastPickUpGame/InteractSystem/ItemSystem/FPUGWorldItemScore.h"

void AFPUGGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SpawnItems();

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

	if (TeamsInfo.IsValidIndex(TeamId))
	{
		TeamsInfo[TeamId] += ScoreToAdd;
	}
	else
	{
		TeamsInfo.EmplaceAt(TeamId, ScoreToAdd);
	}
}

void AFPUGGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	NewPlayer->PlayerState->SetPlayerId(GetNumPlayers() - 1);
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

	TArray<int32> ChosenItemIndexes;

	do
	{
		int32 RandomIndex = FMath::RandRange(1, 10);

		ChosenItemIndexes.AddUnique(RandomIndex);

	} while (ChosenItemIndexes.Num() < 4);

	FString Context;

	for (auto CurrentItemIndex : ChosenItemIndexes)
	{
		for (size_t i = 0; i < 10; i++)
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


