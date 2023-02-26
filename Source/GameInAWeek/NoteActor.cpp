// Fill out your copyright notice in the Description page of Project Settings.


#include "NoteActor.h"

#include "SongActor.h"

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
	
	OwningActor = Cast<ASongActor>(GetOwner());
	
	if(OwningActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Actor Owner Correct"));
	}
}

// Called every frame
void ANoteActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(OwningActor)
	{
		FVector Translation = FVector::XAxisVector;
		Translation.X += OwningActor->GetSongPosition(); 
		AddActorWorldOffset(Translation);
	}
}

