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
	
protected:
	virtual void BeginPlay() override;

private:
	void GameOver();
	
	void ChangeWidget(TSubclassOf<UUserWidget> WidgetClass);
	
private:
	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> BasicHUDClass;

	UPROPERTY(EditAnywhere, Category = "User Interface")
	TSubclassOf<UUserWidget> GameOverClass;


	UPROPERTY()
		UUserWidget* Widget;

	UPROPERTY()
		int CurrentScore = 0;
	
	UPROPERTY()
		bool bIsPlaying;

	UPROPERTY()
		int Misses;

	UPROPERTY()
		bool bIsGameOver;
};



