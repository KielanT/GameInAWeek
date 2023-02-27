// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "RhythmGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class GAMEINAWEEK_API ARhythmGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
    
public:
    ARhythmGameModeBase();

	UFUNCTION(BlueprintCallable)
		bool IsPlaying();

	UFUNCTION(BlueprintCallable)
		void StartPlayGame();

protected:
	virtual void BeginPlay() override;
	
private:
	UPROPERTY()
		bool bIsPlaying;
};
