// Fill out your copyright notice in the Description page of Project Settings.


#include "RhythmGameModeBase.h"

ARhythmGameModeBase::ARhythmGameModeBase()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameInAWeek/Blueprints/BP_RhythmCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

