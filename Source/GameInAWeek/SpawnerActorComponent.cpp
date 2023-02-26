// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActorComponent.h"
#include "SongActor.h"
#include "NoteActor.h"

// Sets default values for this component's properties
USpawnerActorComponent::USpawnerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USpawnerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ASongActor>(GetOwner());

	
	if(!SongData.IsEmpty())
	{
		MaxIndex = SongData.Num() - 1;
		Index = 0;
	}

	if(NoteActorClass != nullptr)
	{
		FActorSpawnParameters Params;
		Params.Owner = GetOwner();
		FTransform trans = GetOwner()->GetTransform();
		FVector vec = trans.GetLocation();
		vec.X = 0;
		trans.SetLocation(vec);
		NoteActor = GetWorld()->SpawnActor<ANoteActor>(NoteActorClass, trans, Params);
	}
	
}


// Called every frame
void USpawnerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Owner)
	{
		SongPosition = Owner->GetSongPosition();
	}
	
	if(!SongData.IsEmpty())
	{
		if(Index <= MaxIndex)
		{
			if(SongData[Index].Position <= SongPosition)
			{
				UE_LOG(LogTemp, Warning, TEXT("Song Pos %f, Location: %s"), SongPosition, *NoteActor->GetActorLocation().ToString());
				UE_LOG(LogTemp, Warning, TEXT("Hit Pos %f"), SongData[Index].Position);
				UE_LOG(LogTemp, Warning, TEXT("Song Pos %f"), SongPosition);
				
				Index++;
			}

		}
	}
}

