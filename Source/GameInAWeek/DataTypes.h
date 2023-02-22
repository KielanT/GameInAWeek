// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataTypes.generated.h"


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

