// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameInAWeekGameMode.h"

#include "GameDataSaveGame.h"
#include "GameInAWeekCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/UEdMode.h"

AGameInAWeekGameMode::AGameInAWeekGameMode()
{
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameInAWeek/Blueprints/BP_BasicCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
	
	bIsGameOver = false;
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->bIsMusic = true;
	
	
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

FString AGameInAWeekGameMode::GetHighScore()
{
	const FString OutScore = FString::FromInt(HighScore);
	
	return "High Score: " + OutScore;
}

void AGameInAWeekGameMode::Pause()
{
	
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	AudioComponent->SetPaused(true);
	bIsPlaying = false;

	if(PauseWidgetClass)
	{
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseWidgetClass); // Creates widget
		if (PauseWidget != nullptr) 
		{
			PauseWidget->AddToViewport(); // Adds widget to viewport
		}
	}
}

void AGameInAWeekGameMode::Resume()
{
	EnableGameInput(true);
	AudioComponent->SetPaused(false);
	bIsPlaying = true;

	if(PauseWidget)
	{
		PauseWidget->RemoveFromParent();
	}
}

void AGameInAWeekGameMode::BeginPlay()
{
	Super::BeginPlay();

	bIsPlaying = false;
	
	ChangeWidget(BasicHUDClass);

	FString slot = "HighScoreSlot";
	if(UGameDataSaveGame* LoadedGame = Cast<UGameDataSaveGame>(UGameplayStatics::LoadGameFromSlot(slot, 0)))
	{
		HighScore = LoadedGame->HighScore;
	}
	else
	{
		HighScore = 0;
	}
}

void AGameInAWeekGameMode::GameOver()
{
	if(CurrentScore > HighScore)
	{
		HighScore = CurrentScore;
		if(UGameDataSaveGame* SaveGame = Cast<UGameDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameDataSaveGame::StaticClass())))
		{
			SaveGame->HighScore = HighScore;
			UGameplayStatics::AsyncSaveGameToSlot(SaveGame, SaveGame->SaveSlotName, SaveGame->UserIndex);
		}
	}

	
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
