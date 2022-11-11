// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGDoorInteractable.h"

EInteractActionType AFPUGDoorInteractable::Interact(AActor* Executor)
{
	ToggleDoorState();

	return EInteractActionType::None;
}

bool AFPUGDoorInteractable::CanInteract(AActor* Executor)
{
	return true;
}
