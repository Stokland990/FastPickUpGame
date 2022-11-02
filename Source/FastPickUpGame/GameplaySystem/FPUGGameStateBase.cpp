// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameStateBase.h"

#include "Net/UnrealNetwork.h"
//#include "Net/PushModel.h"
#include "Engine/DataTable.h"


TArray<int32>& AFPUGGameStateBase::GetScoreInfo()
{
	return TeamScores;
}

int32 AFPUGGameStateBase::GetTimeRemain() const
{
	return TimeRemain;
}

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

void AFPUGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{


	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;
	//SharedParams.Condition = COND_OwnerOnly;

	DOREPLIFETIME(AFPUGGameStateBase, TeamScores);

	//DOREPLIFETIME_CONDITION(AFPUGGameStateBase, TimeRemain, COND_InitialOnly);


	DOREPLIFETIME_WITH_PARAMS_FAST(AFPUGGameStateBase, TimeRemain, SharedParams);

}
