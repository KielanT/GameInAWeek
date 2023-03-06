// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "DataTypes.h"
#include "MainGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEINAWEEK_API UMainGameInstance : public UGameInstance
{
	GENERATED_BODY()

	// Persistant data carried between each scene, this allows the sword to be set in main menu then spawned in, in game
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESwordTypes SwordTypes = ESwordTypes::Foil;
};
