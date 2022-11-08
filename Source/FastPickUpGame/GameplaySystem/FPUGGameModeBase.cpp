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

void AFPUGGameModeBase::InitNewSpawnPoint(AActor* SpawnPointToAdd)
{
	SpawnPoints.AddUnique(SpawnPointToAdd);
}

void AFPUGGameModeBase::InitNewTechDoor(AFPUGDoorTechnical* DoorToAdd)
{
	TechDoors.AddUnique(DoorToAdd);
}

void AFPUGGameModeBase::AddScoreToTeamById(int32 TeamId, int32 ScoreToAdd)
{
	if (!GetGameStateInternal())
	{
		return;
	}

	TArray<int32>& TeamsInfo = GS->GetScoreInfo();

	if (TeamsInfo.IsEmpty())
	{
		TeamsInfo.SetNum(GetNumPlayers());
	}

	TeamsInfo[TeamId] += ScoreToAdd;

	GS->OnRep_TeamScores();
}


void AFPUGGameModeBase::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);

	const int32 NewPlayerId = GetNumPlayers() - 1;

	NewPlayer->PlayerState->SetPlayerId(NewPlayerId);

	const auto PlayerChar = NewPlayer->GetPawn();

	IFPUGPickUpInterface* PickUpInterface = Cast<IFPUGPickUpInterface>(PlayerChar);

	if (PickUpInterface)
	{
		PickUpInterface->SetItemIdToCollect(GetItemIdsInCurrentMatch()[NewPlayerId]);
	}

	HandleNewPlayerInMatch();
}

void AFPUGGameModeBase::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	if (GetNumPlayers() == 2)
	{
		ErrorMessage = "Too many players in game";
	}
	
}

void AFPUGGameModeBase::UpdateMatchTimer()
{
	if (!GetGameStateInternal())
	{
		return;
	}

	const int32 UpdatedTime = GS->GetTimeRemain() - 1;

	GS->SetTimeRemain(UpdatedTime);

	if (UpdatedTime <= 0)
	{
		EndMatch();
	}
}

void AFPUGGameModeBase::EndMatch()
{
	if (!GetGameStateInternal())
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(MatchTimer);

	int32 WinnerId;
	int32 Score = -1;
	bool bIsDraw = true;

	for(uint8 i = 0; i < GS->GetScoreInfo().Num(); i++)
	{
		if (i == 0)
		{
			Score = GS->GetScoreInfo()[i];
		}
		else
		{
			if (Score != GS->GetScoreInfo()[i])
			{
				bIsDraw = false;

				break;
			}
		}
	}

	if (bIsDraw)
	{
		WinnerId = -1;
	}
	else
	{
		UKismetMathLibrary::MaxOfIntArray(GS->GetScoreInfo(), WinnerId, Score);
	}

	GS->SetWinnerId(WinnerId);

	FTimerHandle RestartTimer;

	GetWorld()->GetTimerManager().SetTimer(RestartTimer, this, &AFPUGGameModeBase::RestartMatch, 5.f);
}

void AFPUGGameModeBase::SpawnItems()
{
	if (!GetGameStateInternal() || !GS->GetItemsDT())
	{
		return;
	}

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

void AFPUGGameModeBase::RestartMatch()
{
	const UWorld* World = GetWorld();

	if (!World)
	{
		return;
	}

	const FString MapPath = "/Game/01_Levels/ThirdPersonMap";

	GetWorld()->ServerTravel(MapPath);
}

void AFPUGGameModeBase::HandleNewPlayerInMatch()
{
	if (!HasMatchStarted && GetNumPlayers() >= 2)
	{
		HasMatchStarted = true;

		GetWorld()->GetTimerManager().SetTimer(MatchTimer, this, &AFPUGGameModeBase::StartMatch, 3.f, false);
	}
}

void AFPUGGameModeBase::StartMatch()
{
	if (!GetGameStateInternal())
	{
		return;
	}

	GS->InitTimeRemain();

	GetWorld()->GetTimerManager().SetTimer(MatchTimer, this, &AFPUGGameModeBase::UpdateMatchTimer, 1.f, true);

	OpenTechDoors();

}

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


