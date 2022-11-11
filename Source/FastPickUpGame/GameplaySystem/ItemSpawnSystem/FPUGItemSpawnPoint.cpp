// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGItemSpawnPoint.h"

#include "Engine/World.h"

#include "FastPickUpGame/GameplaySystem/FPUGGameModeBase.h"

// Sets default values
AFPUGItemSpawnPoint::AFPUGItemSpawnPoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

//Initialize spawn point in GameMode
void AFPUGItemSpawnPoint::BeginPlay()
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
		GM->InitNewSpawnPoint(this);
	}
}



