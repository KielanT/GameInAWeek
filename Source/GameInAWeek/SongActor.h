// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SongActor.generated.h"


UCLASS()
class GAMEINAWEEK_API ASongActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASongActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



	UFUNCTION()
		void PlayBackPercetage(const USoundWave* PlayingSoundWave, const float PlaybackPercent);

	UFUNCTION()
		void OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent);
		
	float GetSongPosition();

	float GetSongSpeed();

	float GetLoaderSongPosition();

	float GetLoaderSongSpeed();
	
private:
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;

		UPROPERTY(EditAnywhere)
    		UAudioComponent* AudioLoaderComponent;

	UPROPERTY(EditAnywhere)
		class USpawnerActorComponent* SpawnerComponent;

	UPROPERTY()
		float SongPosition;

	UPROPERTY()
		float SongLoaderPosition;

	float testTime = 0;
};
