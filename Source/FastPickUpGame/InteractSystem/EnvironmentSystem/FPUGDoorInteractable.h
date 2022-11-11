// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPUGDoorBase.h"

#include "FastPickUpGame/InteractSystem/FPUGInteractInterface.h"

#include "FPUGDoorInteractable.generated.h"

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGDoorInteractable : public AFPUGDoorBase, public IFPUGInteractInterface
{
	GENERATED_BODY()

public:

	//Override InteractInterface functions

	EInteractActionType Interact(AActor* Executor) override;

	bool CanInteract(AActor* Executor) override;
};
