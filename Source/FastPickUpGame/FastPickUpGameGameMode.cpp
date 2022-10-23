// Copyright Epic Games, Inc. All Rights Reserved.

#include "FastPickUpGameGameMode.h"
#include "FastPickUpGameCharacter.h"
#include "UObject/ConstructorHelpers.h"

AFastPickUpGameGameMode::AFastPickUpGameGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
