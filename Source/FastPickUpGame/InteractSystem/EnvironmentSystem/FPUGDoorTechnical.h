// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FPUGDoorBase.h"
#include "FPUGDoorTechnical.generated.h"

/**
 * 
 */
UCLASS()
class FASTPICKUPGAME_API AFPUGDoorTechnical : public AFPUGDoorBase
{
	GENERATED_BODY()

protected:

	void BeginPlay() override;
};
