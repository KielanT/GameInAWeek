// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FallingActor.generated.h"

class UBoxComponent;
UCLASS()
class GAMEINAWEEK_API AFallingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFallingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void SetSpeed(float speed);

	void EnableGravity();
	
private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY()
		FVector Velocity = FVector(-10.0f, 0.0f, 0.0f);

	UPROPERTY()
		float Speed = 0;
};
