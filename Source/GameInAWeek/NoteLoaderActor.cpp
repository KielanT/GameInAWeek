// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteLoaderActor.h"

#include "RhythmGameModeBase.h"
#include "SongActor.h"
#include "Kismet/GameplayStatics.h"

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
		Translation.X *= OwningActor->GetLoaderSongPosition() * 50.0f;// * OwningActor->GetLoaderSongSpeed();
		AddActorWorldOffset(Translation * DeltaTime);
	}
}

