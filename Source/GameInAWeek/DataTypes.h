// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes.generated.h"

// A Seperate data type class, which allows just adding this header rather than
// adding the object header when only this class is needed

// Enum class used in game instance (no need for sword actor to be added)
// Enum class used in sword actor
// Enum class can be set in blueprints
UENUM(BlueprintType)
enum class ESwordTypes : uint8
{
	Select = 0,
	Foil,
	Epee,
	Sabre,
	Excalibur,
	Katana
};

