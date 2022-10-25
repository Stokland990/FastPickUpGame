// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGGameStateBase.h"

#include "Net/UnrealNetwork.h"

void AFPUGGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPUGGameStateBase, TeamScores);

}

TArray<int32>& AFPUGGameStateBase::GetScoreInfo()
{
	return TeamScores;
}
