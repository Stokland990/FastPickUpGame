// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPUGGameModeBase.generated.h"

class AFPUGGameStateBase;

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	void AddScoreToTeamById(int32 TeamId, int32 ScoreToAdd);

protected:

	void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	AFPUGGameStateBase* GetGameStateInternal();

private:

	AFPUGGameStateBase* GS;

};
