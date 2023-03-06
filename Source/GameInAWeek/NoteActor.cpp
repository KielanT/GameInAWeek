// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteActor.h"

#include "RhythmGameModeBase.h"
#include "SongActor.h"
#include "Kismet/GameplayStatics.h"

// The note actor is what would be hit if the rhythm game worked currently

// Sets default values
ANoteActor::ANoteActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
}

// Called when the game starts or when spawned
void ANoteActor::BeginPlay()
{
	Super::BeginPlay();
	
	OwningActor = Cast<ASongActor>(GetOwner()); // Used to get the song speed and position
	GameModeRef = Cast<ARhythmGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ANoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// If worked this would be called on the play state started
	//if(OwningActor && GameModeRef->IsPlaying())
	//{
	//	FVector Translation = -FVector::XAxisVector; // Sets the direction to be towards the player
	
	//  Moves the note by song position and speed (this is how rhythm games are supposed to work
	//	Translation.X *= OwningActor->GetSongPosition() * OwningActor->GetSongSpeed(); 
	//	AddActorWorldOffset(Translation);
	//}
}

