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
	IFPUGPickUpInterface* PickUpInterface = Cast<IFPUGPickUpInterface>(Executor);

	if (PickUpInterface)
	{
		const int32 DesiredItemId = PickUpInterface->GetItemIdToCollect();

		if (ItemID == DesiredItemId)
		{
			return true;
		}
	}

	return false;
}
