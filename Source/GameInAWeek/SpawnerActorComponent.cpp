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

	TopRow = CreateDefaultSubobject<USceneComponent>(TEXT("Top Row"));
	CentreRow = CreateDefaultSubobject<USceneComponent>(TEXT("Centre Row"));
	BottomRow = CreateDefaultSubobject<USceneComponent>(TEXT("Bottom Row"));
	
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
	}
}


// Called every frame
void USpawnerActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(Owner)
	{
		SongPosition = Owner->GetLoaderSongPosition();
	}
	
	if(!SongData.IsEmpty())
	{
		if(Index <= MaxIndex)
		{
			
			if(SongData[Index].SongPosition <= SongPosition)
			{
				FActorSpawnParameters Params;
				Params.Owner = GetOwner();
				FTransform trans = GetOwner()->GetTransform();
				FVector vec = trans.GetLocation();
				vec.X = NoteLoaderActor->GetActorLocation().X;

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

