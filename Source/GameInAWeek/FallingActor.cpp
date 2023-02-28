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
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Test Static Mesh"));
	StaticMeshComponent->SetSimulatePhysics(true);
	SetRootComponent(StaticMeshComponent);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("HitSound"));
	
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();

	GameMode = Cast<AGameInAWeekGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if(!ActorDatas.IsEmpty())
	{
		const int index = FMath::RandRange(0, ActorDatas.Num() - 1);
		ActorData = ActorDatas[index];
		StaticMeshComponent->SetStaticMesh(ActorData.Mesh);
		StaticMeshComponent->SetWorldScale3D(ActorData.Scale);
		Score = ActorData.Score;

		ActorDatas.Empty(); // Destroys the other meshes
	}
	
	StaticMeshComponent->SetEnableGravity(false);
	
	IsHit = false;
	
	
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	Speed = speed;
}

void AFallingActor::Hit()
{

	if(ActorData.Type != EObjectType::Bomb)
	{
		if(!AudioComponent->IsPlaying() && !HitSounds.IsEmpty())
		{
			const int Index = FMath::RandRange(0, HitSounds.Num() - 1);
			USoundBase* Sound = HitSounds[Index];
			AudioComponent->SetSound(Sound);
			AudioComponent->Play();
		}
		if(!IsHit)
		{
			GameMode->IncreaseScore(Score);
			StaticMeshComponent->SetEnableGravity(true);
			StaticMeshComponent->AddForce(FVector(1250.0f, 0.0f, 0.0f));
		}
	}
	else
	{
		AudioComponent->SetSound(BombSound);
		AudioComponent->Play();
		GameMode->GameOver("You Hit A bomb! Dodge the bombs");
	}

		
		
	IsHit = true;
		
		
}

void AFallingActor::Missed()
{
	StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, this, &AFallingActor::Death, DeathTime, false);
	
}

EObjectType AFallingActor::GetType()
{
	return ActorData.Type;
}

void AFallingActor::Death()
{
	Destroy();
}





