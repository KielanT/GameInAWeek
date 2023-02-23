// Fill out your copyright notice in the Description page of Project Settings.


#include "SafteyNetTriggerBox.h"
#include "FallingActor.h"

ASafteyNetTriggerBox::ASafteyNetTriggerBox()
{
	
}

void ASafteyNetTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ASafteyNetTriggerBox::OnOverlapBegin);
}

void ASafteyNetTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	UE_LOG(LogTemp, Warning, TEXT("Collision"));
	if(OverlappedActor->GetClass() == TSubclassOf<AFallingActor>())
	{
		UE_LOG(LogTemp, Warning, TEXT("Collision"));
		OverlappedActor->Destroy();
	}
}
