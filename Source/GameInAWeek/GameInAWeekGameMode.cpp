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
	
	bIsGameOver = false;
}

void AGameInAWeekGameMode::IncreaseScore(int score)
{
	CurrentScore += score;
}

int AGameInAWeekGameMode::GetScore()
{
	return CurrentScore;
}

bool AGameInAWeekGameMode::IsPlaying()
{
	return bIsPlaying;
}

void AGameInAWeekGameMode::StartPlayGame()
{
	bIsPlaying = true;
	EnableGameInput(true);
}

void AGameInAWeekGameMode::Missed(bool hasMissed)
{
	if(hasMissed)
	{
		Misses++;
	}
	else
	{
		if(Misses > 0)
			Misses--;
	}

	if(Misses >= 3)
	{
		GameOver();
	}
}

bool AGameInAWeekGameMode::IsGameOver()
{
	return bIsGameOver;
}

void AGameInAWeekGameMode::EnableGameInput(bool Enable)
{
	if(Enable)
	{
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = false;
	}else
	{
		GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeUIOnly());
		GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	}
}

FString AGameInAWeekGameMode::GetFinalScore()
{
	const FString OutScore = FString::FromInt(CurrentScore);

	return "Score: " + OutScore;
}

void AGameInAWeekGameMode::BeginPlay()
{
	Super::BeginPlay();

	bIsPlaying = false;
	
	ChangeWidget(BasicHUDClass);
}

void AGameInAWeekGameMode::GameOver()
{
	ChangeWidget(GameOverClass);
	bIsPlaying = false;
	bIsGameOver = true;
	EnableGameInput(false);
}

void AGameInAWeekGameMode::ChangeWidget(TSubclassOf<UUserWidget> WidgetClass)
{
	if (Widget != nullptr) // Checks if  widget is null or not
	{
		Widget->RemoveFromParent(); // Removes widget from viewport
		Widget = nullptr; // Sets widget to null
	}
	if (WidgetClass != nullptr) // Checks if widget class is null or not
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass); // Creates widget
		if (Widget != nullptr) 
		{
			Widget->AddToViewport(); // Adds widget to viewport
		}
	}
}
