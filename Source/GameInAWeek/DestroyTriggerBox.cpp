// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyTriggerBox.h"

ADestroyTriggerBox::ADestroyTriggerBox()
{
	
}

void ADestroyTriggerBox::BeginPlay()
{
	Super::BeginPlay();
	OnActorBeginOverlap.AddDynamic(this, &ADestroyTriggerBox::OnOverlapBegin);
}

void ADestroyTriggerBox::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	// Destroys any actor that overlaps
	OtherActor->Destroy();
}
