// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameModeBase.generated.h"

class UUserWidget;
UCLASS()
class GAMEINAWEEK_API AMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMenuGameModeBase();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "User Interface")
		TSubclassOf<UUserWidget> MenuClass;

	UPROPERTY()
		UUserWidget* Widget;
};
