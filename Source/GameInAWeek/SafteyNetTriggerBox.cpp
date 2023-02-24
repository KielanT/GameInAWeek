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
	if(OtherActor->GetClass()->IsChildOf(AFallingActor::StaticClass()))
	{
		AFallingActor* actor = Cast<AFallingActor>(OtherActor);
		//actor->EnableGravity();
		OtherActor->Destroy();
	}
}
