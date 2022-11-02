// Fill out your copyright notice in the Description page of Project Settings.


#include "FPUGWorldItemScore.h"

#include "FastPickUpGame/CharacterSystem/FPUGPickUpInterface.h"

EInteractActionType AFPUGWorldItemScore::Interact(AActor* Executor)
{
	Super::Interact(Executor);

	return EInteractActionType::ScorePickUp;
}

bool AFPUGWorldItemScore::CanInteract(AActor* Executor)
{
	IFPUGPickUpInterface* PickUpIterface = Cast<IFPUGPickUpInterface>(Executor);

	if (PickUpIterface)
	{
		int32 DesiredItemId = PickUpIterface->GetItemIdToCollect();

		if (ItemID == DesiredItemId)
		{
			return true;
		}
	}

	return false;
}
