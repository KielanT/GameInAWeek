// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteLoaderActor.h"

#include "RhythmGameModeBase.h"
#include "SongActor.h"
#include "Kismet/GameplayStatics.h"

// This is used to play the song before the game starts to load in all the notes in the correct position
// However since this moves by song position and speed, the notes get spawned in difference places can be off from as little as .1 or over a 100
// Even when using delta time the spawns are off 

// Sets default values
ANoteLoaderActor::ANoteLoaderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANoteLoaderActor::BeginPlay()
{
	Super::BeginPlay();
	OwningActor = Cast<ASongActor>(GetOwner());
	GameModeRef = Cast<ARhythmGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ANoteLoaderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if(OwningActor)
	{
		FVector Translation = FVector::XAxisVector;

		// Attempted solution was to spawn in song position by a set speed
		Translation.X *= OwningActor->GetLoaderSongPosition() * 50.0f;// * OwningActor->GetLoaderSongSpeed();
		AddActorWorldOffset(Translation * DeltaTime);
	}
}

