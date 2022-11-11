// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameStateBase.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/HUD.h"
#include "GameFramework/PlayerState.h"

#include "FastPickUpGame/GameplaySystem/PlayerControlSystem/FPUGHUDInterface.h"


TArray<int32>& AFPUGGameStateBase::GetScoreInfo()
{
	return TeamScores;
}

int32 AFPUGGameStateBase::GetTimeRemain() const
{
	return TimeRemain;
}

//This is called just once on the start of a match. Starts timer on client side.
void AFPUGGameStateBase::InitTimeRemain()
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AFPUGGameStateBase, TimeRemain, this);

	TimeRemain = MatchTime;

	OnRep_TimeRemain();
}

void AFPUGGameStateBase::SetTimeRemain(const int32 NewTime)
{
	TimeRemain = NewTime;
}

void AFPUGGameStateBase::SetWinnerId(const int32 WinnerId)
{
	MARK_PROPERTY_DIRTY_FROM_NAME(AFPUGGameStateBase, WinnerPlayerId, this);

	WinnerPlayerId = WinnerId;

	OnRep_WinnerPlayerId();
}

UDataTable* AFPUGGameStateBase::GetItemsDT()
{
	return ItemsDT;
}

void AFPUGGameStateBase::OnRep_TeamScores()
{
	OnScoreUpdated.Broadcast(TeamScores);
}

void AFPUGGameStateBase::OnRep_TimeRemain()
{
	OnInitTime.Broadcast(TimeRemain);
}

void AFPUGGameStateBase::OnRep_WinnerPlayerId()
{
	for (const auto CurrentPS : PlayerArray)
	{
		const auto PC = CurrentPS->GetPlayerController();

		if(!PC)
		{
			continue;
		}

		const auto CurrentHUD = PC->GetHUD();

		const auto HUDInterface = Cast<IFPUGHUDInterface>(CurrentHUD);

		if (HUDInterface)
		{
			HUDInterface->EndMatchNotify(WinnerPlayerId);
		}
	}
}

void AFPUGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	SharedParams.Condition = COND_None;

	DOREPLIFETIME(AFPUGGameStateBase, TeamScores);

	DOREPLIFETIME_WITH_PARAMS_FAST(AFPUGGameStateBase, TimeRemain, SharedParams);
	DOREPLIFETIME_WITH_PARAMS_FAST(AFPUGGameStateBase, WinnerPlayerId, SharedParams);

}
