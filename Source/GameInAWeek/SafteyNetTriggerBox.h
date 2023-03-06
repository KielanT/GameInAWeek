// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "SafteyNetTriggerBox.generated.h"

/**
 * 
 */
UCLASS()
class GAMEINAWEEK_API ASafteyNetTriggerBox : public ATriggerBox
{
	GENERATED_BODY()

public:
	ASafteyNetTriggerBox();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor); 
};
