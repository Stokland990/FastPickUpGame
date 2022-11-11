// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ItemSystem/FPUGItemData.h"

#include "FPUGInteractExecutorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UFPUGInteractExecutorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FASTPICKUPGAME_API IFPUGInteractExecutorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Returns component which will be used for a trace
	virtual USceneComponent* GetComponentForInteractTrace() const = 0;

	//Handles interact on executor side using Interact Type
	virtual void HandleInteractAction(EInteractActionType Type) = 0;
};
