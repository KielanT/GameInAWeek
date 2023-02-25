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
	
	void Hit();
	void Missed();
	
	
private:
	UFUNCTION()
		void Death();

	void SwapMeshes();
	
private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;

	

	UPROPERTY(EditAnywhere)
		FVector MovementDirection = FVector(-1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
		FRotator RotationDirection = FRotator(1.0f, 0.0f, 0.0f);

	UPROPERTY()
		float Speed = 0;

	UPROPERTY(EditAnywhere)
		TArray<UStaticMesh*> BallMeshes;

	UPROPERTY(EditAnywhere)
		UStaticMesh* BallMesh;

	UPROPERTY()
		bool IsHit;

	UPROPERTY()
		bool IsMissed;

	UPROPERTY()
		FTimerHandle DeathTimerHandle;

	UPROPERTY(EditAnywhere)
		float DeathTime = 4.0f;
	
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = Audio)
		TArray<USoundBase*> HitSounds;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> DestructionMesh;
};
