// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameInAWeekGameMode.h"
#include "GameInAWeekCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGameInAWeekGameMode::AGameInAWeekGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameInAWeek/Blueprints/BP_BasicCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
