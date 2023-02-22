// Fill out your copyright notice in the Description page of Project Settings.


#include "SwordActor.h"

#include "MainGameInstance.h"

// Sets default values
ASwordActor::ASwordActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sword"));
	SetRootComponent(StaticMeshComponent);
}

// Called when the game starts or when spawned
void ASwordActor::BeginPlay()
{
	Super::BeginPlay();

	UMainGameInstance* GameInstance = Cast<UMainGameInstance>(GetGameInstance());
	if(GameInstance)
	{
		Key = GameInstance->SwordTypes;
	}
	for(auto& sword : SwordMap)
	{
		if(sword.Key == Key)
		{
			SelectedMesh = sword.Value;
		}
	}
	
	if(SelectedMesh)
		StaticMeshComponent->SetStaticMesh(SelectedMesh);
}

// Called every frame
void ASwordActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

