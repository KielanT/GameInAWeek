// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActorComponent.h"
#include "SongActor.h"
#include "NoteActor.h"
#include "NoteLoaderActor.h"

// Used for spawning the notes in the song actor

// Sets default values for this component's properties
USpawnerActorComponent::USpawnerActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Sets the spawn rows 
	TopRow = CreateDefaultSubobject<USceneComponent>(TEXT("Top Row"));
	CentreRow = CreateDefaultSubobject<USceneComponent>(TEXT("Centre Row"));
	BottomRow = CreateDefaultSubobject<USceneComponent>(TEXT("Bottom Row"));
	
}


// Called when the game starts
void USpawnerActorComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<ASongActor>(GetOwner());

	// Spawns the loader note on begin play to load in the song
	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	FTransform Trans = GetOwner()->GetTransform();
	
	if(NoteLoaderClass)
	{
		NoteLoaderActor = GetWorld()->SpawnActor<ANoteLoaderActor>(NoteLoaderClass, Trans, Params);
	}

	// Sets the indexes if the song data exists
	if(!SongData.IsEmpty())
	{
		MaxIndex = SongData.Num() - 1;
		Index = 0;
	}
}


// Called every frame
void USpawnerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Sets the song position
	if(Owner)
	{
		SongPosition = Owner->GetLoaderSongPosition();
	}
	
	if(!SongData.IsEmpty())
	{
		// Spawns the correct note when at the song position
		if(Index <= MaxIndex)
		{
			
			if(SongData[Index].SongPosition <= SongPosition) // == Equals will never get hit since using floats some of the positions get missed. 
			{
				FActorSpawnParameters Params;
				Params.Owner = GetOwner();
				FTransform trans = GetOwner()->GetTransform();
				FVector vec = trans.GetLocation();
				vec.X = NoteLoaderActor->GetActorLocation().X;

				// Sets the note to spawn in the correct row
				if(SongData[Index].Row == 1) 
				{
					vec.Z = TopRow->GetComponentTransform().GetLocation().Z;
				}
				else if(SongData[Index].Row == 2)
				{
					vec.Z = CentreRow->GetComponentTransform().GetLocation().Z;
				}
				else if(SongData[Index].Row == 3)
				{
					vec.Z = BottomRow->GetComponentTransform().GetLocation().Z;
				}
				trans.SetLocation(vec);
				ANoteActor* NoteActor = GetWorld()->SpawnActor<ANoteActor>(NoteActorClass, trans, Params);
				
				Index++;
			}
		}
	}
}

