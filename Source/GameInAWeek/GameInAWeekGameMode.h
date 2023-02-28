// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameInAWeekGameMode.generated.h"

UCLASS(minimalapi)
class AGameInAWeekGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameInAWeekGameMode();

	void IncreaseScore(int score);

	UFUNCTION(BlueprintCallable)
		int GetScore();

	UFUNCTION(BlueprintCallable)
		bool IsPlaying();

	UFUNCTION(BlueprintCallable)
		void StartPlayGame();

	UFUNCTION()
		void Missed(bool hasMissed);

	UFUNCTION(BlueprintCallable)
		bool IsGameOver();

	UFUNCTION(BlueprintCallable)
		void EnableGameInput(bool Enable);

	UFUNCTION(BlueprintCallable)
		FString GetFinalScore();

	UFUNCTION(BlueprintCallable)
		FString GetHighScore();
	
	UFUNCTION(BlueprintCallable)
		void Pause();

	UFUNCTION(BlueprintCallable)
		void Resume();

	void GameOver();
	
protected:
	virtual void BeginPlay() override;

private:
	
	
	void ChangeWidget(TSubclassOf<UUserWidget> WidgetClass);
	
	
	
private:
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;
	
	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> BasicHUDClass;

	UPROPERTY(EditAnywhere, Category = "User Interface")
	TSubclassOf<UUserWidget> GameOverClass;

	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> PauseWidgetClass;

	UPROPERTY()
		UUserWidget* Widget;

	UPROPERTY()
		UUserWidget* PauseWidget;

	UPROPERTY()
		int CurrentScore = 0;
	
	UPROPERTY()
		bool bIsPlaying;

	UPROPERTY()
		int Misses;

	UPROPERTY()
		bool bIsGameOver;

	UPROPERTY()
		int HighScore;
};



