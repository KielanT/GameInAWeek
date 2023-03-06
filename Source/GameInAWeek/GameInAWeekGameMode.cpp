// Copyright Epic Games, Inc. All Rights Reserved.

#include "GameInAWeekGameMode.h"

#include "GameDataSaveGame.h"
#include "GameInAWeekCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

/*Game mode for the infinite game used to control the gameplay*/

AGameInAWeekGameMode::AGameInAWeekGameMode()
{
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/GameInAWeek/Blueprints/BP_BasicCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class; // Sets the default player
	}
	
	// Creates an audio component for playing music in the background
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->bIsMusic = true;

	// Sets default values
	bIsGameOver = false;
	ReasonText = "";
}

void AGameInAWeekGameMode::BeginPlay()
{
	Super::BeginPlay();

	// Sets default value
	bIsPlaying = false;

	// Sets the HUD Widget
	ChangeWidget(BasicHUDClass);

	// If the game has a load file, load it and set the highscore if not the highscore is 0
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

void AGameInAWeekGameMode::IncreaseScore(int score)
{
	CurrentScore += score; // Increases the score
}

int AGameInAWeekGameMode::GetScore()
{
	return CurrentScore; // Gets the score and allows it to be show in a widget
}

bool AGameInAWeekGameMode::IsPlaying()
{
	return bIsPlaying; // Used to check if the game is in a play state
}

void AGameInAWeekGameMode::StartPlayGame()
{
	// Starts the game and enabled input for game only
	bIsPlaying = true;
	EnableGameInput(true);
}


void AGameInAWeekGameMode::PlayerHit()
{
	// If the player is hit increment the hits variable
	Hits++;

	// If the player has been hit then end game
	if(Hits >= 3)
	{
		GameOver("You were hit too many times! Hit or Dodge the balls");
	}
}

bool AGameInAWeekGameMode::IsGameOver()
{
	return bIsGameOver; // Used for checking if the game is over
}

void AGameInAWeekGameMode::EnableGameInput(bool Enable)
{
	// If true then enable input for game only and hides the cursor
	// If false then disable input for game and sets it for UI and shows cursor
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
	// Returns the Score has a string 
	const FString OutScore = FString::FromInt(CurrentScore);
	return "Score: " + OutScore;
}

FString AGameInAWeekGameMode::GetHighScore()
{
	// Returns the highscore has a string
	const FString OutScore = FString::FromInt(HighScore);
	return "High Score: " + OutScore;
}

int AGameInAWeekGameMode::GetHits()
{
	// Gets the hits allow to be seen in widget
	// Currently shows the number would be better to have hit images (to look more polished)
	return Hits; 
}

void AGameInAWeekGameMode::Pause()
{
	// On Pause sets the input to game and UI, sets to game to allow the pause buttons to be pressed again
	GetWorld()->GetFirstPlayerController()->SetInputMode(FInputModeGameAndUI());
	GetWorld()->GetFirstPlayerController()->bShowMouseCursor = true;
	AudioComponent->SetPaused(true);// Pauses the music
	bIsPlaying = false; // Stops the play state

	// Shows pause menu
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
	// Reenable the game and remove the pause meny
	EnableGameInput(true);
	AudioComponent->SetPaused(false);
	bIsPlaying = true;
	
	if(PauseWidget)
	{
		PauseWidget->RemoveFromParent();
		PauseWidget = nullptr;
	}
}

FString AGameInAWeekGameMode::GetReason()
{
	return ReasonText; // Allows the reason text to be called in widget
}



void AGameInAWeekGameMode::GameOver(FString reasonText)
{
	ReasonText = reasonText; // Sets the reason text on game over
	AudioComponent->Stop(); // Stops the audio
	
	if(CurrentScore > HighScore) // Checks if the new score is higher than current score then saves it to file
	{
		HighScore = CurrentScore;
		if(UGameDataSaveGame* SaveGame = Cast<UGameDataSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameDataSaveGame::StaticClass())))
		{
			SaveGame->HighScore = HighScore;
			UGameplayStatics::AsyncSaveGameToSlot(SaveGame, SaveGame->SaveSlotName, SaveGame->UserIndex);
		}
	}
	
	ChangeWidget(GameOverClass); // Shows the game over widget instead of HUD
	bIsPlaying = false; // Stops the game from playing (physics still simulate)
	bIsGameOver = true; // Sets game to be over
	EnableGameInput(false); // Disables game input

	
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
