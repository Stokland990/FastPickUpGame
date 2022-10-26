// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameStateBase.h"

#include "Net/UnrealNetwork.h"
#include "Engine/DataTable.h"


TArray<int32>& AFPUGGameStateBase::GetScoreInfo()
{
	return TeamScores;
}

int32 AFPUGGameStateBase::GetTimeRemain() const
{
	return TimeRemain;
}

void AFPUGGameStateBase::SetTimeRemain(const int32 NewTime)
{
	TimeRemain = NewTime;
}

UDataTable* AFPUGGameStateBase::GetItemsDT()
{
	return ItemsDT;
}

void AFPUGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPUGGameStateBase, TeamScores);

	DOREPLIFETIME(AFPUGGameStateBase, TimeRemain);
}
