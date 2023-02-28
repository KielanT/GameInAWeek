// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "FallingActor.generated.h"

UENUM(BlueprintType)
enum class EObjectType : uint8
{
	Select = 0,
	Ball,
	Bomb
};

USTRUCT(BlueprintType)
struct FActorData
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere)
		UStaticMesh* Mesh;

	UPROPERTY(EditAnywhere)
		EObjectType Type;

	UPROPERTY(EditAnywhere)
		int Score = 0;

	UPROPERTY(EditAnywhere)
		FVector Scale = FVector(1.0f, 1.0f, 1.0f);
	
	
};

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

	UPROPERTY()
		class AGameInAWeekGameMode* GameMode;

	UPROPERTY(EditAnywhere)
		FVector MovementDirection = FVector(-1.0f, 0.0f, 0.0f);

	UPROPERTY(EditAnywhere)
		FRotator RotationDirection = FRotator(1.0f, 0.0f, 0.0f);

	UPROPERTY()
		float Speed = 0;
	
	UPROPERTY(EditAnywhere, Category = Data)
		TArray<FActorData> ActorDatas;

	UPROPERTY()
		FActorData ActorData;

	UPROPERTY()
		int Score;

	UPROPERTY()
		bool IsHit;

	UPROPERTY()
		bool IsMissed;

	UPROPERTY()
		FTimerHandle DeathTimerHandle;

	UPROPERTY(EditAnywhere)
		float DeathTime = 2.0f;
	
	UPROPERTY(EditAnywhere)
		UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = Audio)
		TArray<USoundBase*> HitSounds;
	
	UPROPERTY(EditAnywhere, Category = Audio)
		USoundBase* MissedSound;

	UPROPERTY(EditAnywhere, Category = Audio)
		USoundBase* BombSound;
	
	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> DestructionMesh;
};
