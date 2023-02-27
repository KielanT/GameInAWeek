// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerActorComponent.h"
#include "SongActor.h"
#include "NoteActor.h"
#include "NoteLoaderActor.h"

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

	FActorSpawnParameters Params;
	Params.Owner = GetOwner();
	FTransform trans = GetOwner()->GetTransform();
	
	if(NoteLoaderClass)
	{
		NoteLoaderActor = GetWorld()->SpawnActor<ANoteLoaderActor>(NoteLoaderClass, trans, Params);
	}
	
	if(!SongData.IsEmpty())
	{
		MaxIndex = SongData.Num() - 1;
		Index = 0;

		if(NoteActorClass != nullptr)
		{
			for(auto data : SongData)
			{
				FVector vec = trans.GetLocation();
				vec.X = data.WorldPositionX;
				trans.SetLocation(vec);
				ANoteActor* NoteActor = GetWorld()->SpawnActor<ANoteActor>(NoteActorClass, trans, Params);
			}
		}
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
	
	//if(!SongData.IsEmpty())
	//{
	//	if(Index <= MaxIndex)
	//	{
	//		if(SongData[Index].SongPosition <= SongPosition)
	//		{
	//			UE_LOG(LogTemp, Warning, TEXT("Song Pos %f, Location: %s"), SongPosition, *NoteActor->GetActorLocation().ToString());
	//			UE_LOG(LogTemp, Warning, TEXT("Actual beat Pos %f"), SongData[Index].SongPosition);
	//			
	//			Index++;
	//		}
//
	//	}
	//}
}

