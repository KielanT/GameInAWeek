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

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		ESwordTypes SwordTypes = ESwordTypes::Foil;
};
