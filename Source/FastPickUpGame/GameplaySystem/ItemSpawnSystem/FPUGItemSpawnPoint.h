// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "FPUGItemSpawnPoint.generated.h"

UCLASS()
class FASTPICKUPGAME_API AFPUGItemSpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPUGItemSpawnPoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;



};
