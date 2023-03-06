// Fill out your copyright notice in the Description page of Project Settings.


#include "SongActor.h"

#include "AudioDevice.h"
#include "SpawnerActorComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
ASongActor::ASongActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Audio component to play the main song
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->bAlwaysPlay = false;
	AudioComponent->bIsMusic = true;
	AudioComponent->bStopWhenOwnerDestroyed = true;
	AudioComponent->bAutoActivate = false;

	// Audio Component to load in the notes
	AudioLoaderComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Loader"));
	AudioLoaderComponent->bIsMusic = true;
	AudioLoaderComponent->bStopWhenOwnerDestroyed = true;
	AudioLoaderComponent->bAutoActivate = false;

	// Spawner component to spawn in the notes
	SpawnerComponent = CreateDefaultSubobject<USpawnerActorComponent>(TEXT("Spawner"));


}

// Called when the game starts or when spawned
void ASongActor::BeginPlay()
{
	Super::BeginPlay();

	// Allows the song actor to calculate the songs position
	AudioComponent->OnAudioPlaybackPercent.AddDynamic(this, &ASongActor::PlayBackPercetage);
	AudioLoaderComponent->OnAudioPlaybackPercent.AddDynamic(this, &ASongActor::OnAudioPlaybackPercent);
	AudioLoaderComponent->Play();
}

// Called every frame
void ASongActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASongActor::PlayBackPercetage(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	SongPosition = PlaybackPercent * PlayingSoundWave->Duration; // Calculates the song position
}

void ASongActor::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	SongLoaderPosition = PlaybackPercent * PlayingSoundWave->Duration;
}

float ASongActor::GetSongPosition()
{
	return SongPosition; // Gets the song position
}

float ASongActor::GetSongSpeed()
{
	return AudioComponent->PitchMultiplier; // Unreal engine uses the pitch for the speed of the song (so does unity)
}

float ASongActor::GetLoaderSongPosition()
{
	return SongLoaderPosition;
}

float ASongActor::GetLoaderSongSpeed()
{
	return AudioLoaderComponent->PitchMultiplier;
}


