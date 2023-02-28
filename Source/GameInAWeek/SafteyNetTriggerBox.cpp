// Fill out your copyright notice in the Description page of Project Settings.


#include "SafteyNetTriggerBox.h"
#include "FallingActor.h"

// Safety net actor has changed a lot through out development and is now used to tell the actor if the player has missed it or not

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
	if(OtherActor->GetClass()->IsChildOf(AFallingActor::StaticClass()))
	{
		AFallingActor* Actor = Cast<AFallingActor>(OtherActor);
		if(Actor)
		{
			Actor->Missed();
		}
		else
		{
			OtherActor->Destroy();
		}
	}
}
