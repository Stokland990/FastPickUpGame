// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "FPUGPlayerStateBase.generated.h"

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGPlayerStateBase : public APlayerState
{
	GENERATED_BODY()
	
protected:

	void BeginPlay() override;
};
