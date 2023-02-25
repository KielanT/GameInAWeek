// Fill out your copyright notice in the Description page of Project Settings.


#include "BallsGeometryCollectionActor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

ABallsGeometryCollectionActor::ABallsGeometryCollectionActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABallsGeometryCollectionActor::BeginPlay()
{
	Super::BeginPlay();
	if(GeometryCollection)
	{
		GeometryCollectionComponent->SetRestCollection(GeometryCollection);
		
		GeometryCollectionComponent->SetWorldScale3D(FVector(4.0f, 4.0f, 4.0f));

		if(MaterialDestruct)
		{
			GeometryCollectionComponent->SetPhysMaterialOverride(MaterialDestruct);
		}
		
		UE_LOG(LogTemp, Warning, TEXT("Setted RestGeom"));
	}
}
