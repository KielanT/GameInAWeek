// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DataTypes.h"
#include "SwordActor.generated.h"

UCLASS()
class GAMEINAWEEK_API ASwordActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwordActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* StaticMeshComponent;
	
	UPROPERTY(EditAnywhere, Category = Sword)
		ESwordTypes Key;
	
	UPROPERTY(EditAnywhere, Category = Sword)
		TMap<ESwordTypes, UStaticMesh*> SwordMap;
	
	UPROPERTY()
		UStaticMesh* SelectedMesh;

};
