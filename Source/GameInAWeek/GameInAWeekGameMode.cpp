// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameInAWeekGameMode.h"
#include "GameInAWeekCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

AGameInAWeekGameMode::AGameInAWeekGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameInAWeek/Blueprints/BP_BasicCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AGameInAWeekGameMode::IncreaseScore(int score)
{
	CurrentScore += score;
}

int AGameInAWeekGameMode::GetScore()
{
	return CurrentScore;
}

void AGameInAWeekGameMode::BeginPlay()
{
	Super::BeginPlay();

	if(BasicHUDClass)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), BasicHUDClass);
		if(Widget)
		{
			Widget->AddToViewport();
		}
	}
}
