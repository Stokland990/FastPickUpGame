// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGDoorTechnical.h"

#include "FastPickUpGame/GameplaySystem/FPUGGameModeBase.h"

void AFPUGDoorTechnical::BeginPlay()
{
	Super::BeginPlay();

	const auto World = GetWorld();

	if (!World)
	{
		return;
	}

	const auto GM = Cast<AFPUGGameModeBase>(World->GetAuthGameMode());

	if (GM)
	{
		GM->InitNewTechDoor(this);
	}
}
