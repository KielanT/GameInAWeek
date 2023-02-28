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
	
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio"));
	AudioComponent->bAlwaysPlay = false;
	AudioComponent->bIsMusic = true;
	AudioComponent->bStopWhenOwnerDestroyed = true;
	AudioComponent->bAutoActivate = false;

	AudioLoaderComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Loader"));
	AudioLoaderComponent->bIsMusic = true;
	AudioLoaderComponent->bStopWhenOwnerDestroyed = true;
	AudioLoaderComponent->bAutoActivate = false;
	
	SpawnerComponent = CreateDefaultSubobject<USpawnerActorComponent>(TEXT("Spawner"));


}

// Called when the game starts or when spawned
void ASongActor::BeginPlay()
{
	Super::BeginPlay();
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
	SongPosition = PlaybackPercent * PlayingSoundWave->Duration;
	//UE_LOG(LogTemp, Warning, TEXT("Playback %f"), SongPosition);
}

void ASongActor::OnAudioPlaybackPercent(const USoundWave* PlayingSoundWave, const float PlaybackPercent)
{
	SongLoaderPosition = PlaybackPercent * PlayingSoundWave->Duration;
	//UE_LOG(LogTemp, Warning, TEXT("Playback Loader, %f"), SongLoaderPosition);
}

float ASongActor::GetSongPosition()
{
	return SongPosition;
}

float ASongActor::GetSongSpeed()
{
	return AudioComponent->PitchMultiplier;
}

float ASongActor::GetLoaderSongPosition()
{
	return SongLoaderPosition;
}

float ASongActor::GetLoaderSongSpeed()
{
	return AudioLoaderComponent->PitchMultiplier;
}

