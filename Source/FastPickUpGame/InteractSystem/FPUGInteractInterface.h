// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "ItemSystem/FPUGItemData.h"

#include "FPUGInteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, meta = (CannotImplementInterfaceInBlueprint))
class UFPUGInteractInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class FASTPICKUPGAME_API IFPUGInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//Checks if Executor can interact with object
	UFUNCTION(BlueprintCallable, Category = "Interact")
	virtual bool CanInteract(AActor* Executor) = 0;

	UFUNCTION(BlueprintCallable, Category = "Interact")
	virtual EInteractActionType Interact(AActor* Executor) = 0;
};
