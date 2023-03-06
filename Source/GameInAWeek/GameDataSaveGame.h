// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameDataSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class GAMEINAWEEK_API UGameDataSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UGameDataSaveGame();

	// Data to be saved
	UPROPERTY(VisibleAnywhere, Category = Basic)
		int HighScore;

	// Save file name
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;

	// Save file index
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
};
