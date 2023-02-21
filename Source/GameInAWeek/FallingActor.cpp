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


	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	CollisionBox->SetCollisionProfileName("Trigger");
	CollisionBox->SetupAttachment(StaticMeshComponent);
}

// Called when the game starts or when spawned
void AFallingActor::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AFallingActor::OnBeginOverlap);
	
}

// Called every frame
void AFallingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallingActor::OnBeginOverlap(UPrimitiveComponent* OverlapComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Floor"))
	{
		//Destroy();
	}
}




