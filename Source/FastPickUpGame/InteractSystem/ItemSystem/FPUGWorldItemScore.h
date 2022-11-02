// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPUGWorldItemBase.h"
#include "FPUGWorldItemScore.generated.h"

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGWorldItemScore : public AFPUGWorldItemBase
{
	GENERATED_BODY()

public:

	//Override InteractInterface functions

	EInteractActionType Interact(AActor* Executor) override;

	bool CanInteract(AActor* Executor) override;
	
};
