// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGWorldItemScore.h"

#include "FastPickUpGame/CharacterSystem/FPUGPickUpInterface.h"

void AFPUGWorldItemScore::Interact(AActor* Executor)
{
	IFPUGPickUpInterface* PickUpIterface = Cast<IFPUGPickUpInterface>(Executor);

	if (PickUpIterface)
	{
		PickUpIterface->PickUpScoreItem(ScoreValue);
	}

	Super::Interact(Executor);
}
