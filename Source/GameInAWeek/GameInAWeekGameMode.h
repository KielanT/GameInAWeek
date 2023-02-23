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
	
protected:
	virtual void BeginPlay() override;

	
private:
	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> BasicHUDClass;

	UPROPERTY()
		UUserWidget* Widget;
	
	int CurrentScore = 0;
};



