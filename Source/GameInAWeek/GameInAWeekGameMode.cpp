// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameInAWeekGameMode.h"
#include "GameInAWeekCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameInAWeekGameMode::AGameInAWeekGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
