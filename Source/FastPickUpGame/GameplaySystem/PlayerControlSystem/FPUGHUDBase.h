// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"

#include "FPUGHUDInterface.h"

#include "FPUGHUDBase.generated.h"

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGHUDBase : public AHUD, public IFPUGHUDInterface
{
	GENERATED_BODY()

public:

	void EndMatchNotify(int32 TeamWon) override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Gameplay")
	void ShowEndMatchResult(int32 TeamWon);
};
