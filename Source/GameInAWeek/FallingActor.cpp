// Fill out your copyright notice in the Description page of Project Settings.


#include "FallingActor.h"

#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameInAWeekGameMode.h"

// Sets default values
AFallingActor::AFallingActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Creates the static mesh component
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Static Mesh"));
	StaticMeshComponent->SetSimulatePhysics(true);
	SetRootComponent(StaticMeshComponent);

	// Creates the Audio component for when it gets hit
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGameInAWeekGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	// If the actor has data set the actor data than delete the rest to free up memory
	// Would be better if spawner actor had control over this
	if(!ActorDatas.IsEmpty())
	{
		const int index = FMath::RandRange(0, ActorDatas.Num() - 1); // Chooses a random actor data to choose
		ActorData = ActorDatas[index];
		StaticMeshComponent->SetStaticMesh(ActorData.Mesh);
		StaticMeshComponent->SetWorldScale3D(ActorData.Scale); // The balls (not the bomb) all require scaling up
		Score = ActorData.Score; // Sets the amount of points that the object is worth

		ActorDatas.Empty(); // Destroys the other meshes
	}

	// Disables gravity but allows the physics to still be on
	StaticMeshComponent->SetEnableGravity(false);

	// Set the default values
	IsHit = false;
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Only moves towards the player if the game is in play state and it has not been hit
	if(GameMode->IsPlaying())
	{
		if(!IsHit)
		{
			FVector Translation = MovementDirection * Speed * DeltaTime;
			AddActorWorldOffset(Translation);

			FRotator Rotator = RotationDirection * Speed * DeltaTime;
			AddActorWorldRotation(Rotator);
		}
	}
}

void AFallingActor::SetSpeed(float speed)
{
	Speed = speed; // Sets the speed (controlled by the spawner)
}

void AFallingActor::Hit()
{
	// If the actor is not a bomb then play hit sound and increase score
	if(ActorData.Type != EObjectType::Bomb)
	{
		if(!AudioComponent->IsPlaying() && !HitSounds.IsEmpty())
		{
			// Sets a random sound since there are multiple hit sounds (would have been better to use a sound cue)
			const int Index = FMath::RandRange(0, HitSounds.Num() - 1); 
			USoundBase* Sound = HitSounds[Index];
			AudioComponent->SetSound(Sound);
			AudioComponent->Play();
		}
		if(!IsHit)
		{
			GameMode->IncreaseScore(Score);
			StaticMeshComponent->SetEnableGravity(true); // Enables gravity
			
			StaticMeshComponent->AddForce(FVector(1250.0f, 0.0f, 0.0f));  	// Adds a force to the ball to show the ball has been hit
		}
	}
	else
	{
		// If is a bomb then play bomb sound and end the game
		AudioComponent->SetSound(BombSound);
		AudioComponent->Play();
		GameMode->GameOver("You Hit A bomb! Dodge the bombs");
	}
	
	IsHit = true; // Sets that this object has been hit
}

void AFallingActor::Missed()
{
	// If the player has missed (if the object has gone through a safety net actor)
	// Then change collision to physics only, this stops the player from rotating whenever hit by a ball
	// Then the death timer gets set to destroy the ball after a certain amount of time to help stop the game becoming laggy
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AFallingActor::Death, DeathTime, false);
}

EObjectType AFallingActor::GetType()
{
	return ActorData.Type; // Returns the type
}

void AFallingActor::Death()
{
	Destroy(); // Destroys this actor
}





