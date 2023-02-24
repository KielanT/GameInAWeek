// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingActor.h"
#include "Components/BoxComponent.h"

// Sets default values
AFallingActor::AFallingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Static Mesh"));
	StaticMeshComponent->SetSimulatePhysics(true);
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();
	StaticMeshComponent->SetEnableGravity(false);
	
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Translation = Velocity * Speed * DeltaTime;
	AddActorWorldOffset(Translation);
}

void AFallingActor::SetSpeed(float speed)
{
	Speed = speed;
}

void AFallingActor::EnableGravity()
{
	StaticMeshComponent->SetEnableGravity(true);
}





