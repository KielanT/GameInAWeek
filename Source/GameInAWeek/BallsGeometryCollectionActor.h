// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeometryCollection/GeometryCollectionActor.h"
#include "BallsGeometryCollectionActor.generated.h"

class UGeometryCollection;
/**
 * 
 */
UCLASS()
class GAMEINAWEEK_API ABallsGeometryCollectionActor : public AGeometryCollectionActor
{
	GENERATED_BODY()

public:
	ABallsGeometryCollectionActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
		UGeometryCollection* GeometryCollection;

	UPROPERTY(EditAnywhere)
		UPhysicalMaterial* MaterialDestruct;
};
