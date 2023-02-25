// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingActor.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"


// Sets default values
AFallingActor::AFallingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Static Mesh"));
	StaticMeshComponent->SetSimulatePhysics(true);
	SetRootComponent(StaticMeshComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();
	if(!BallMeshes.IsEmpty())
	{
		const int index = FMath::RandRange(0, BallMeshes.Num() - 1);
		BallMesh = BallMeshes[index];
		StaticMeshComponent->SetStaticMesh(BallMesh);
		BallMeshes.Empty(); // Destroys the other meshes
	}

	StaticMeshComponent->SetStaticMesh(BallMesh);
	
	StaticMeshComponent->SetEnableGravity(false);
	
	
	IsHit = false;
	IsMissed = false;
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(!IsHit || !IsMissed)
	{
		FVector Translation = MovementDirection * Speed * DeltaTime;
		AddActorWorldOffset(Translation);

		FRotator Rotator = RotationDirection * Speed * DeltaTime;
		AddActorWorldRotation(Rotator);
	}
}

void AFallingActor::SetSpeed(float speed)
{
	Speed = speed;
}

void AFallingActor::Hit()
{
	if(!IsMissed)
	{
		if(!AudioComponent->IsPlaying() && !HitSounds.IsEmpty())
		{
			const int Index = FMath::RandRange(0, HitSounds.Num() - 1);
			USoundBase* Sound = HitSounds[Index];
			AudioComponent->SetSound(Sound);
			AudioComponent->Play();
		}
		
		
		IsHit = true;
		StaticMeshComponent->SetEnableGravity(true);
		StaticMeshComponent->AddForce(FVector(1250.0f, 0.0f, 0.0f));
	}
	//GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AFallingActor::Death, DeathTime, false);
	
}

void AFallingActor::Missed()
{
	IsMissed = true;
	StaticMeshComponent->SetEnableGravity(true);
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SwapMeshes();
}

void AFallingActor::Death()
{
	Destroy();
}

void AFallingActor::SwapMeshes()
{
	if(DestructionMesh)
	{
		FActorSpawnParameters Params;
		GetWorld()->SpawnActor<AActor>(DestructionMesh, GetTransform(), Params);
	}
}



