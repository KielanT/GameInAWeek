// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SpawnerActorComponent.generated.h"

class ANoteLoaderActor;
class ANoteActor;

USTRUCT(BlueprintType)
struct FSongData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		float SongPosition;

	UPROPERTY(EditAnywhere)
		float WorldPositionX;

	UPROPERTY(EditAnywhere)
		int Row;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GAMEINAWEEK_API USpawnerActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerActorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:

	UPROPERTY(EditAnywhere)
		USceneComponent* TopRow;

	UPROPERTY(EditAnywhere)
		USceneComponent* CentreRow;

	UPROPERTY(EditAnywhere)
		USceneComponent* BottomRow;

	UPROPERTY(EditAnywhere)
		TArray<FSongData> SongData;
	
	UPROPERTY()
		class ASongActor* Owner;

	float SongPosition;

	int Index;

	int MaxIndex;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ANoteActor> NoteActorClass;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ANoteLoaderActor> NoteLoaderClass;

	UPROPERTY()
		ANoteLoaderActor* NoteLoaderActor;
};
