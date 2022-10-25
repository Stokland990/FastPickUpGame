// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameModeBase.h"

#include "GameFramework/PlayerState.h"

#include "FPUGGameStateBase.h"

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
