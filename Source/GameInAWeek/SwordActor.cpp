// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordActor.h"

#include "FallingActor.h"
#include "Components/BoxComponent.h"
#include "MainGameInstance.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASwordActor::ASwordActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SetRootComponent(StaticMeshComponent);

	// Creates the collision box
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(StaticMeshComponent);

}

// Called when the game starts or when spawned
void ASwordActor::BeginPlay()
{
	Super::BeginPlay();

	// Gets the Game instance to search and spawn the correct sword
	if(UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance()))
	{
		Key = GameInstance->SwordTypes;
	}
	SelectedMesh = *SwordMap.Find(Key);

	// Sets the sowrd mesh
	if(SelectedMesh)
	{
		StaticMeshComponent->SetStaticMesh(SelectedMesh);
	}

	// Sets the collision bind
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASwordActor::OnBeginOverlap);
}

// Called every frame
void ASwordActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwordActor::OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// Checks if the object has a tag (this is not really needed anymore
	// Should be replaced by checking if the actor is the correct class
	if(OtherActor->ActorHasTag(TEXT("Object")))
	{
		AFallingActor* Actor = Cast<AFallingActor>(OtherActor);

		// If the player is lunging and hit the actor then tell the actor its been hit
		if(Actor && bIsPlayerLunging)
		{
			Actor->Hit();
		}
		else if(!Actor)
		{
			OtherActor->Destroy();
		}
		
	}
}

